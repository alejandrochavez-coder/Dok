import xml.etree.ElementTree as etree
from dataclasses import dataclass
from pathlib import Path
from enum import Enum
import urllib.request

class VulkanApi(Enum):
    VULKAN = "vulkan"
    VULKAN_SC = "vulkansc"

class CommandType(Enum):
    Global = "{Global}"
    Instance = "{Instance}"
    Device = "{Device}"

@dataclass
class Feature:
    requirement: str
    commands: list[str]

@dataclass
class CommandEntry:
    requirement: str
    command: str

@dataclass
class ParsePath:
    source: Path
    target: Path

VULKAN_URL = "https://raw.githubusercontent.com/KhronosGroup/Vulkan-Docs/main/xml/vk.xml"

def get_registry() -> etree.ElementTree:
    with urllib.request.urlopen(VULKAN_URL) as file:
        return etree.parse(file)

def parse_tree_entries(commands: list[etree.Element[str]], requirement: str) -> list[CommandEntry]:

    entries: list[CommandEntry] = []

    for command in commands:

        command_name = command.get("name")
        entries.append(CommandEntry(requirement, command_name))

    return entries

def parse_feature_entries(registry: etree.ElementTree, api: VulkanApi) -> list[CommandEntry]:
    entries: list[CommandEntry] = []

    for feature in registry.findall("feature"):

        if api.value not in feature.get("api").split(","):
            continue

        feature_name = feature.get("name")

        requirement = feature_name.replace("BASE_", "").replace("COMPUTE_", "").replace("GRAPHICS_", "")

        commands = feature.findall("require/command")
        entries.extend(parse_tree_entries(commands, requirement))

    return entries

def parse_spec_dependency(require: etree.Element[str], author: str) -> str:

    spec = require.find("enum")
    if author == "KHR" or spec is None:
        return ""   

    spec_version = spec.get("value", "")
    spec_name = spec.get("name", "")

    if spec_name and spec_version and int(spec_version) > 1:
        return f"{spec_name} >= {spec_version}"

    return ""

def parse_extension_entries(registry: etree.ElementTree, api: VulkanApi) -> list[CommandEntry]:
    entries: list[CommandEntry] = []

    for extension in registry.findall("extensions/extension"):

        if api.value not in extension.get("supported").split(",") or extension.get("promotedto", "") or extension.get("deprecatedby", ""):
            continue

        extension_name = extension.get("name")
        extension_author = extension.get("author")

        for require in extension.findall("require"):

            dependencies = require.get("depends", "")
            dependencies = f"({dependencies})" if "," in dependencies else dependencies
            dependencies = f"+{dependencies}" if dependencies else dependencies

            spec_version = parse_spec_dependency(require, extension_author)
            spec_version = f"+{spec_version}" if spec_version else spec_version

            requirement = extension_name + dependencies + spec_version

            commands = require.findall("command")
            entries.extend(parse_tree_entries(commands, requirement))

    return entries

def parse_command_types(registry: etree.ElementTree) -> dict[str, CommandType]:
    command_types: dict[str, CommandType] = {}
    type_handles: dict[str, CommandType] = {}
    type_parents: dict[str, str] = {}

    for type in registry.findall("types/type"):
        if type.get("category") == "handle" and (type_name := type.findtext("name")):
            type_parents[type_name] = type.get("parent", type_name)

    for type_name in type_parents.keys(): 

        command_type = None
        handle_name = type_name

        while command_type is None:

            if handle_name == "VkInstance": command_type = CommandType.Instance
            if handle_name == "VkDevice": command_type = CommandType.Device
            handle_name = type_parents[handle_name]

        type_handles[type_name] = command_type

    for command in registry.find("commands"):
        
        command_name = command.findtext("proto/name")
        if command_name == "vkGetInstanceProcAddr":
            command_types[command_name] = CommandType.Global
            continue

        if command_name == "vkGetDeviceProcAddr":
            command_types[command_name] = CommandType.Instance
            continue

        type_name = command.findtext("param[1]/type")
        command_types[command_name] = type_handles.get(type_name, CommandType.Global)

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

        command_type = command_types.get(entry.command)
        if command_type is None:
            continue

        type_feature_entries.setdefault(command_type, {}).setdefault(entry.requirement, []).append(entry.command)

    entries: dict[CommandType, list[Feature]] = {}

    for type, feature_entries in type_feature_entries.items():

        features: list[Feature] = []

        for requirement, commands in feature_entries.items():

            feature = Feature(requirement, commands)
            features.append(feature)

        entries[type] = features

    return entries

def parse_requirement(requirement: str) -> str:

    buffer = ""
    parsed = ""

    for char in requirement:
        if char not in "()+,":
            buffer += char
            continue

        parsed += buffer if not buffer or "=" in buffer else f"defined({buffer})"

        buffer = ""
        parsed += char

    parsed += buffer if not buffer or "=" in buffer else f"defined({buffer})"
    return parsed.replace("+", " && ").replace(",", " || ")

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

        patched += f"#endif //{feature.requirement}\n"

    return patched

def parse_file(entries: dict[CommandType, list[Feature]], parse_file: ParsePath):

    patched_data = ""

    with open(parse_file.source, "r") as file:

        for line in file.readlines():
            patched_data += patch_file_line(line, entries)

    with open(parse_file.target, "w") as file:
        file.write(patched_data)
        
directory = Path(__file__).parent
registry = get_registry()
entries = parse_handle_entries(registry, VulkanApi.VULKAN)

parse_paths = [
    ParsePath(directory / "dok_unparched.c", directory / "dok.c"),
    ParsePath(directory / "dok_unparched.h", directory / "dok.h")
]

for parse_path in parse_paths:
    parse_file(entries, parse_path)