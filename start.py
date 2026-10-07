import xml.etree.ElementTree as etree
from dataclasses import dataclass
from typing import Callable
from enum import Enum
import urllib.request
import re

class VulkanApi(Enum):
    VULKAN = "vulkan"
    VULKAN_SC = "vulkansc"

class RequirementJoint(Enum):
    AND = "+"
    OR = ","

class CommandType(Enum):
    Global = "GLOBAL"
    Instance = "INSTANCE"
    Device = "DEVICE"

@dataclass
class FeatureEntry:
    requirement: str
    command: str

@dataclass
class CommandEntry:
    requirement: str
    command: str
    command_type: CommandType

VULKAN_URL = "https://raw.githubusercontent.com/KhronosGroup/Vulkan-Docs/main/xml/vk.xml"

def get_registry() -> etree.ElementTree:
    with urllib.request.urlopen(VULKAN_URL) as file:
        return etree.parse(file)

def parse_tree_entries(commands: list[etree.Element[str]], requirement: str) -> list[FeatureEntry]:

    entries: list[FeatureEntry] = []

    for command in commands:

        command_name = command.get("name")
        if not command_name:
            continue

        entries.append(FeatureEntry(requirement, command_name))

    return entries

def parse_feature_entries(registry: etree.ElementTree, api: VulkanApi) -> list[FeatureEntry]:
    entries: list[FeatureEntry] = []

    for feature in registry.findall("feature"):

        if api.value not in feature.get("api").split(","):
            continue

        requirement = re.sub(r"VK_(BASE|COMPUTE|GRAPHICS)_VERSION_", "VK_VERSION_", feature.get("name"))

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

    if author == "KHR":
        return ""

    spec = require.find("enum")
    if spec is None:
        return ""

    spec_name = spec.get("name", "")
    if not spec_name:
        return ""
    
    spec_version = spec.get("value", "")
    if not spec_version:
        return ""
    
    if int(spec_version) > 0:
        return ""
    
    return f"{spec_name} >= {spec_version}"


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

        if buffer and symbol or (last and not symbol) and not enum:
            parsed += f"defined({buffer})"

        if symbol and buffer:
            buffer = ""

        if symbol:
            parsed += char

    return parsed.replace("+", " && ").replace(",", " || ")

def get_raw_type_type(type: str, type_parents: dict[str, str | None]) -> CommandType:
    while type:
        if type == "VkInstance":
            return CommandType.Instance
        
        if type == "VkDevice":
            return CommandType.Device
        
        type = type_parents.get(type)

    return CommandType.Global

def get_command_types(registry: etree.ElementTree) -> dict[str, CommandType]:
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
        command_type = get_raw_type_type(type, type_parents)

        command_types[command_name] = command_type

    return command_types

def parse_command_entries(registry: etree.ElementTree, api: VulkanApi):

    entries: list[CommandEntry] = []

    command_requirements: dict[str, list[str]] = {}
    command_types = get_command_types(registry)

    for entry in parse_feature_entries(registry, api) + parse_extension_entries(registry, api):

        command_requirements.setdefault(entry.command, []).append(entry.requirement)

    for command, requirements in command_requirements.items():

        requirement = requirements[0]
        if len(requirements) != 1:
            requirement = join_requirements(requirements, RequirementJoint.OR)

        command_type = command_types[command]   
        entries.append(CommandEntry(requirement, command, command_type))

    return entries
    

def start(registry: etree.ElementTree, api: VulkanApi):

    for entry in parse_command_entries(registry, api):

        print(f"{entry.command_type.name} {entry.command}")
        
registry = get_registry()
start(registry, VulkanApi.VULKAN)