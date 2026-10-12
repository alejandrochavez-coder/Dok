import xml.etree.ElementTree as etree
from dataclasses import dataclass
from pathlib import Path
from enum import Enum
import urllib.request

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

def parse_feature_entries(registry: etree.ElementTree) -> list[CommandEntry]:
    entries: list[CommandEntry] = []

    for feature in registry.findall("feature"):

        if "vulkan" not in feature.get("api").split(","):
            continue

        feature_name = feature.get("name")
        requirement = feature_name.replace("_BASE", "").replace("_COMPUTE", "").replace("_GRAPHICS", "")

        commands = feature.findall("require/command")
        entries.extend(parse_tree_entries(commands, requirement))

    return entries

def parse_spec_version(require: etree.Element[str], author: str) -> str:
    spec = require.find("enum", "")
    if spec is None or author == "KHR" or not (spec_version := spec.get("value")) or int(spec_version) <= 1:
        return ""   

    spec_name = spec.get("name", "")
    return f"+{spec_name} >= {spec_version}"

def parse_extension_entries(registry: etree.ElementTree) -> list[CommandEntry]:
    entries: list[CommandEntry] = []

    for extension in registry.findall("extensions/extension"):

        if "vulkan" not in extension.get("supported").split(",") or extension.get("promotedto", "") or extension.get("deprecatedby", ""):
            continue

        extension_name = extension.get("name")
        author = extension.get("author")

        for require in extension.findall("require"):

            dependencies = require.get("depends", "")
            dependencies = f"({dependencies})" if "," in dependencies else dependencies
            dependencies = f"+{dependencies}" if dependencies else dependencies
            spec_version = parse_spec_version(require, author)

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

def parse_command_entries(registry: etree.ElementTree):

    command_requirements: dict[str, list[str]] = {}
    entries: list[CommandEntry] = []

    for entry in parse_feature_entries(registry) + parse_extension_entries(registry):
        command_requirements.setdefault(entry.command, []).append(entry.requirement)

    for command, requirements in command_requirements.items():

        unique_requirement = len(requirements) == 1
        requirement = requirements[0] if unique_requirement else ",".join(f"({requirement})" for requirement in requirements)

        entry = CommandEntry(requirement, command)
        entries.append(entry)

    return entries

def parse_handle_entries(registry: etree.ElementTree) -> dict[CommandType, list[Feature]]:
    type_feature_entries: dict[CommandType, dict[str, list[str]]] = {}
    command_types = parse_command_types(registry)

    for entry in parse_command_entries(registry):
        if (command_type := command_types.get(entry.command, "")):
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

        parsed += f"defined({buffer})" if buffer and "=" not in buffer else buffer
        parsed += char
        buffer = ""

    parsed += f"defined({buffer})" if buffer and "=" not in buffer else buffer
    return parsed.replace("+", " && ").replace(",", " || ")

def line_command_type(line: str) -> CommandType:

    for command_type in CommandType:
        if command_type.value in line:
            return command_type

    return None

def parse_file(entries: dict[CommandType, list[Feature]], file_path: Path):

    unpatched_data: list[str] = []
    patched_data = ""

    with open(file_path, "r") as file:

        outside_region = True

        for line in file:

            region_ends = line.lstrip().startswith("#pragma endregion")
            region_starts = line.lstrip().startswith("#pragma region ")
            valid_region = region_starts and line_command_type(line) is not None

            outside_region = False if valid_region else outside_region
            outside_region = True if region_ends else outside_region

            if outside_region or region_starts: unpatched_data.append(line)

    for line in unpatched_data:

        command_type = line_command_type(line)
        patched_data += line

        if not line.startswith("#pragma region") or command_type is None: 
            continue

        identifier = line.replace("#pragma region", "", 1).rstrip() + "\n"

        for feature in entries[command_type]:
            patched_data += f"#if {parse_requirement(feature.requirement)} \n"
            patched_data += "".join([identifier.replace(command_type.value, command) for command in feature.commands])
            patched_data += f"#endif // {parse_requirement(feature.requirement)} \n"

    with open(file_path, "w") as file:
        file.write(patched_data)
        
directory = Path(__file__).parent
registry = get_registry()
entries = parse_handle_entries(registry)

file_paths = [
    directory / "dok.c",
    directory / "dok.h"
]

for file_path in file_paths:
    parse_file(entries, file_path)