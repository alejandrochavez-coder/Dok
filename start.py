import xml.etree.ElementTree as etree
from dataclasses import dataclass
from pathlib import Path
from enum import Enum
import urllib.request

class VulkanApi(Enum):
    VULKAN = "vulkan"
    VULKAN_SC = "vulkansc"

class RequirementJoint(Enum):
    AND = "+"
    OR = ","

class CommandType(Enum):
    Global = "{Global}"
    Instance = "{Instance}"
    Device = "{Device}"

@dataclass
class Feature:
    requirement: str
    commands: list[str]

@dataclass
class FeatureEntry:
    requirement: str
    command: str

@dataclass
class CommandEntry:
    requirement: str
    command: str

VULKAN_URL = "https://raw.githubusercontent.com/KhronosGroup/Vulkan-Docs/main/xml/vk.xml"

def get_registry() -> etree.ElementTree:
    with urllib.request.urlopen(VULKAN_URL) as file:
        return etree.parse(file)

def parse_tree_entries(commands: list[etree.Element[str]], requirement: str) -> list[FeatureEntry]:

    entries: list[FeatureEntry] = []

    for command in commands:

        command_name = command.get("name")
        entries.append(FeatureEntry(requirement, command_name))

    return entries

def parse_feature_entries(registry: etree.ElementTree, api: VulkanApi) -> list[FeatureEntry]:
    entries: list[FeatureEntry] = []

    for feature in registry.findall("feature"):

        if api.value not in feature.get("api").split(","):
            continue

        feature_name = feature.get("name")

        requirement = feature_name.replace("BASE_", "").replace("COMPUTE_", "").replace("GRAPHICS_", "")

        commands = feature.findall("require/command")
        entries.extend(parse_tree_entries(commands, requirement))

    return entries

def join_requirements(requirements: list[str], joint: RequirementJoint) -> str:

    valid_requirements: list[str] = []

    contrary_joint = RequirementJoint.AND
    if joint == contrary_joint:
        contrary_joint = RequirementJoint.OR

    for requirement in requirements:

        if not requirement:
            continue

        if contrary_joint.value in requirement:
            valid_requirements.append(f"({requirement})")

        else:

            valid_requirements.append(requirement)

    return joint.value.join(valid_requirements)

def parse_spec_dependency(require: etree.Element[str], author: str) -> str:

    spec = require.find("enum")
    if author == "KHR" or spec is None:
        return ""   

    spec_version = spec.get("value", "")
    spec_name = spec.get("name", "")

    if spec_name and spec_version and int(spec_version) > 1:
        return f"{spec_name} >= {spec_version}"

    return ""

def parse_extension_entries(registry: etree.ElementTree, api: VulkanApi) -> list[FeatureEntry]:
    entries: list[FeatureEntry] = []

    for extension in registry.findall("extensions/extension"):

        if api.value not in extension.get("supported").split(","):
            continue

        extension_name = extension.get("name")
        extension_author = extension.get("author")

        for require in extension.findall("require"):

            command_dependencies = require.get("depends", "")
            spec_dependency = parse_spec_dependency(require, extension_author)

            requirement = join_requirements([extension_name, command_dependencies, spec_dependency], RequirementJoint.AND)

            commands = require.findall("command")
            entries.extend(parse_tree_entries(commands, requirement))

    return entries

def parse_requirement(requirement: str) -> str:

    buffer = ""
    parsed = ""

    for index, char in enumerate(requirement):

        last = index == len(requirement) - 1
        enum = "=" in buffer
        symbol = char in "()+,"

        if not symbol:
            buffer += char

        if (buffer and symbol or last and not symbol) and not enum:
            parsed += f"defined({buffer})"

        if (buffer and symbol or last and not symbol) and enum:
            parsed += buffer

        if buffer and symbol:
            buffer = ""

        if symbol:
            parsed += char

    return parsed.replace("+", " && ").replace(",", " || ")

def get_command_type(type: str, type_parents: dict[str, str | None]) -> CommandType:
    while type:
        if type == "VkInstance":
            return CommandType.Instance
        
        if type == "VkDevice":
            return CommandType.Device
        
        type = type_parents.get(type)

    return CommandType.Global

def parse_command_types(registry: etree.ElementTree) -> dict[str, CommandType]:
    command_types: dict[str, CommandType] = {}
    type_parents: dict[str, str | None] = {}

    for type in registry.findall("types/type"):
        if type.get("category") == "handle" and (command_name := type.findtext("name")):
            type_parents[command_name] = type.get("parent")

    for command in registry.find("commands"):
        alias = command.get("alias")
        if alias:
            command_types[command.get("name")] = command_types[alias]
            continue

        command_name = command.findtext("proto/name")
        if command_name == "vkGetInstanceProcAddr":
            command_types[command_name] = CommandType.Global
            continue

        if command_name == "vkGetDeviceProcAddr":
            command_types[command_name] = CommandType.Instance
            continue

        type = command.findtext("param[1]/type")
        command_type = get_command_type(type, type_parents)

        command_types[command_name] = command_type

    return command_types

def parse_command_entries(registry: etree.ElementTree, api: VulkanApi):

    entries: list[CommandEntry] = []

    command_requirements: dict[str, list[str]] = {}

    for entry in parse_feature_entries(registry, api) + parse_extension_entries(registry, api):
        command_requirements.setdefault(entry.command, []).append(entry.requirement)

    for command, requirements in command_requirements.items():
        if len(requirements) == 1:
            entries.append(CommandEntry(requirements[0], command))
            continue

        requirement = ",".join(f"({requirement})" for requirement in requirements)
        entries.append(CommandEntry(requirement, command))

    return entries

def parse_handle_entries(registry: etree.ElementTree, api: VulkanApi) -> dict[CommandType, list[Feature]]:
    type_feature_entries: dict[CommandType, dict[str, list[str]]] = {}
    command_types = parse_command_types(registry)

    for entry in parse_command_entries(registry, api):

        command_type = command_types[entry.command]
        type_feature_entries.setdefault(command_type, {}).setdefault(entry.requirement, []).append(entry.command)

    entries: dict[CommandType, list[Feature]] = {}

    for type, feature_entries in type_feature_entries.items():

        features: list[Feature] = []

        for requirement, commands in feature_entries.items():

            feature = Feature(requirement, commands)
            features.append(feature)

        entries[type] = features

    return entries

def patch_file_line(line: str, entries: dict[CommandType, list[Feature]]) -> str:

    if not line.lstrip().startswith("//"):

        return line

    found_command_type = None

    for command_type in CommandType:

        if command_type.value in line:

            found_command_type = command_type
            break

    if found_command_type is None:

        return line

    line = line.replace("//", "  ", 1).rstrip() + "\n"
    patched = ""

    for feature in entries[found_command_type]:

        patched += f"#if {parse_requirement(feature.requirement)} \n"

        for command in feature.commands:

            patched += line.replace(command_type.value, command)

        patched += f"#endif \n"

    return patched

def start(registry: etree.ElementTree, api: VulkanApi, file_paths: list[Path]):

    entries = parse_handle_entries(registry, api)

    for file_path in file_paths:

        patched_data = ""
        lines = []

        with open(file_path, "r") as file:

            lines.extend(file.readlines())

        for line in lines:

            patched_data += patch_file_line(line, entries)

        lines.clear()
        print(patched_data)
        # with open(file_path, "w") as file:

        #     file.write(patched_data)
        
registry = get_registry()
# source_file = Path(__file__).parent / "dok_unparched.c"
header_file = Path(__file__).parent / "test.h"

start(registry, VulkanApi.VULKAN, [header_file])