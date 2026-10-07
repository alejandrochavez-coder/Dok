import xml.etree.ElementTree as etree
from dataclasses import dataclass
from typing import Callable
from enum import Enum
import urllib.request
import re

class VulkanApi(Enum):
    VULKAN = "vulkan"
    VULKAN_SC = "vulkansc"

@dataclass
class FeatureEntry:
    requirement: str
    command: str

VULKAN_URL = "https://raw.githubusercontent.com/KhronosGroup/Vulkan-Docs/main/xml/vk.xml"

def get_registry() -> etree.ElementTree:
    with urllib.request.urlopen(VULKAN_URL) as file:
        return etree.parse(file)

def parse_entries(commands: list[etree.Element[str]], requirement: str) -> list[FeatureEntry]:

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
        entries.extend(parse_entries(commands, requirement))

    return entries

# def join_requirements(a: str, b: str) -> str:
#     if not b:
#         return a
    
#     if "," in a:
#         a = f"({a})"

#     if "," in b:
#         b = f"({b})"

#     return f"{a}+{b}"

def join_requirements(requirements: list[str]) -> str:

    valid_requirements: list[str] = []

    for requirement in requirements:

        if not requirement:
            continue

        if "," in requirement:
            valid_requirements.append(f"({requirement})")
        else:
            valid_requirements.append(requirement)

    return "+".join(valid_requirements)        

def parse_extension_entries(registry: etree.ElementTree, api: VulkanApi) -> list[FeatureEntry]:
    entries: list[FeatureEntry] = []

    for extension in registry.findall("extensions/extension"):

        if api.value not in extension.get("supported").split(","):
            continue

        extension_name = extension.get("name")
        extension_dependencies = extension.get("depends")

        for require in extension.findall("require"):

            command_dependencies = require.get("depends", "")

            requirement = join_requirements([extension_name, extension_dependencies, command_dependencies])

            commands = require.findall("command")
            entries.extend(parse_entries(commands, requirement))

    return entries

def start(registry: etree.ElementTree, api: VulkanApi):

    requirement_commands: dict[str, list[str]] = {}
    
    for entry in parse_extension_entries(registry, api):
        requirement_commands.setdefault(entry.requirement, []).append(entry.command)

    for requirement, commands in requirement_commands.items():
        print(requirement)
        print(", ".join(commands))
        print()

registry = get_registry()
start(registry, VulkanApi.VULKAN)
# parse_extension_entries(registry, VulkanApi.VULKAN)