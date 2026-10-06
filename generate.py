import xml.etree.ElementTree as etree
from dataclasses import dataclass
from typing import Callable
from enum import Enum
import urllib.request
import re

VULKAN_URL = "https://raw.githubusercontent.com/KhronosGroup/Vulkan-Docs/main/xml/vk.xml"

class CommandType(Enum):
    Global = "GLOBAL"
    Instance = "INSTANCE"
    Device = "DEVICE"

@dataclass
class Feature:
    requirement: str
    commands: list[str]

def get_registry() -> etree.ElementTree:
    with urllib.request.urlopen(VULKAN_URL) as file:
        return etree.parse(file)
    
def get_requirement_enum_extension(requirement: etree.Element[str], author: str) -> str:
    enum = requirement.find("enum")
    if author == "KHR" or enum is None or not (value := enum.get("value")) or int(value) <= 1:
        return ""
    
    name = enum.get("name")
    return f" && ({name}>={value})"

def get_requirement_depends_extension(requirement: etree.Element[str]) -> str:
    depends = requirement.get("depends")
    if not depends:
        return ""
    if "," in depends:
        return f"+({depends})"
    return f"+{depends}"

def get_extension_command_requirements(extension: etree.Element[str]) -> dict[str, list[str]]:
    supported_apis = extension.get("supported").split(",")
    if "vulkan" not in supported_apis:
        return {}
    extension_name = extension.get("name")
    author = extension.get("author")

    command_requirements: dict[str, list[str]] = {}
    for requirement in extension.findall("require"):
        key = extension_name + get_requirement_depends_extension(requirement)
        key = re.sub(r"[a-zA-Z0-9_]+", lambda m: f"defined({m.group(0)})", key)
        key = key.replace(",", " || ").replace("+", " && ")
        key += get_requirement_enum_extension(requirement, author)
        for command in requirement.findall("command"):
            name = command.get("name")
            command_requirements.setdefault(name, []).append(key)
    return command_requirements

def get_feature_command_requirements(feature: etree.Element[str]) -> dict[str, list[str]]:
    command_requirements: dict[str, list[str]] = {}
    key = feature.get("name")
    key = re.sub(r"VK_(BASE|COMPUTE|GRAPHICS)_VERSION_", "VK_VERSION_", key)
    key = f"defined({key})"
    for command in feature.findall("require/command"):
        name = command.get("name")
        command_requirements.setdefault(name, []).append(key)
    return command_requirements

def get_raw_type_type(raw_type: str, raw_type_parents: dict[str, str | None]):
    while raw_type:
        if raw_type == "VkInstance":
            return CommandType.Instance
        elif raw_type == "VkDevice":
            return CommandType.Device
        raw_type = raw_type_parents.get(raw_type)
    return CommandType.Global

def get_command_types(registry: etree.ElementTree):
    command_types: dict[str, CommandType] = {}
    raw_type_parents: dict[str, str | None] = {}

    for raw_type in registry.findall("types/type"):
        if raw_type.get("category") == "handle" and (name := raw_type.findtext("name")):
            raw_type_parents[name] = raw_type.get("parent")

    for command in registry.find("commands"):
        alias = command.get("alias")
        if alias:
            command_types[command.get("name")] = command_types[alias]
            continue
        name = command.findtext("proto/name")
        raw_type = command.findtext("param[1]/type")
        type = get_raw_type_type(raw_type, raw_type_parents)
        if name == "vkGetInstanceProcAddr":
            type = CommandType.Global
        elif name == "vkGetDeviceProcAddr":
            type = CommandType.Instance
        command_types[name] = type
    return command_types

def get_type_features(registry: etree.ElementTree) -> dict[CommandType, list[Feature]]:
    command_types: dict[str, CommandType] = get_command_types(registry)
    command_requirements: dict[str, list[str]] = {}

    for feature in registry.findall("feature"):
        supported_apis = feature.get("api").split(",")
        if "vulkan" not in supported_apis:
            continue

        for command, requirements in get_feature_command_requirements(feature).items():
            command_requirements.setdefault(command, []).extend(requirements)

    for extension in registry.findall("extensions/extension"):
        supported_apis = extension.get("supported").split(",")
        if "vulkan" not in supported_apis:
            continue

        for command, requirements in get_extension_command_requirements(extension).items():
            command_requirements.setdefault(command, []).extend(requirements)

    type_features: dict[CommandType, dict[str, list[str]]] = {}
    for command, requirements in command_requirements.items():
        if len(requirements) == 1:
            key = requirements[0]
        else:
            key = " || ".join([f"({requirement})" for requirement in requirements])
        type = command_types[command]
        type_features.setdefault(type, {}).setdefault(key, []).append(command)

    parsed_commands: dict[CommandType, list[Feature]] = {}
    for type, features in type_features.items():
        for requirement, commands in features.items():
            parsed_commands.setdefault(type, []).append(Feature(requirement, commands))

    return parsed_commands

def get_feature_data(features: list[Feature], func: Callable[[str], str]) -> str:
    data = []
    for feature in features:
        data.append(f"#if {feature.requirement}")
        for command in feature.commands:
            data.append(func(command))
        data.append(f"#endif // {feature.requirement}")
    return "\n".join(data)

def unpatch_file(file_data: list[str]) -> list[str]:
    unpatch_data = []
    inside_block = False
    for line in file_data:
        if line.startswith("//DOK_START"):
            unpatch_data.append(line)
            inside_block = True
            continue

        if line.startswith("//DOK_END"):
            unpatch_data.append(line)
            inside_block = False
            continue

        if not inside_block:
            unpatch_data.append(line)
    return unpatch_data

def patch_file(name: str, block_data: dict[str, str]):
    unpatched_data: list[str]
    with open(name, "r") as file:
        unpatched_data = unpatch_file(file)
    
    patched_data: list[str] = []
    for data in unpatched_data:
        patched_data.append(data)
        if data.startswith("//DOK_START"):
            key = data.removeprefix("//DOK_START_").strip()
            patched_data.append(block_data.get(key, "") + "\n")

    with open(name, "w") as file:
        file.write("".join(patched_data))

registry = get_registry()
header_data: dict[str, str] = {}
implementation_data: dict[str, str] = {}

for type, features in get_type_features(registry).items():
    key = type.value
    header_data[key] = get_feature_data(features, 
                             lambda command: f"    PFN_{command} {command};")
    implementation_data[key] = get_feature_data(features, 
                                     lambda command: f"    pOutFunctions->{command} = LOAD({command})")

patch_file("dok.c", implementation_data)
patch_file("dok.h", header_data)