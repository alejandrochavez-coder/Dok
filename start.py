import xml.etree.ElementTree as etree
from dataclasses import dataclass
from pathlib import Path
from enum import Enum, auto
import string
import urllib.request

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

class TokenType(Enum):
    ATOM = auto()
    SYMBOL = auto()

@dataclass
class Feature:
    requirement: str
    commands: list[str]

@dataclass
class HandleEntry:
    type: CommandType
    features: list[Feature]

@dataclass
class Token:
    type: TokenType
    value: str

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

    tokenized: list[Token] = tokenize(requirement, list("()+,"))
    parsed = ""

    for token in tokenized:

        if token.type == TokenType.SYMBOL:

            parsed += token.value
            continue

        if "=" in token.value:

            parsed += f"({token.value})"
            continue

        parsed += f"defined({token.value})"

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
    command_types = parse_command_types(registry)

    for entry in parse_feature_entries(registry, api) + parse_extension_entries(registry, api):

        command_requirements.setdefault(entry.command, []).append(entry.requirement)

    for command, requirements in command_requirements.items():

        requirement = requirements[0]
        if len(requirements) != 1:
            requirement = join_requirements(requirements, RequirementJoint.OR)

        command_type = command_types[command]   
        entries.append(CommandEntry(requirement, command, command_type))

    return entries

def parse_handle_entries(registry: etree.ElementTree, api: VulkanApi) -> list[HandleEntry]:
    type_feature_entries: dict[CommandType, dict[str, list[str]]] = {}

    for entry in parse_command_entries(registry, api):

        type_feature_entries.setdefault(entry.command_type, {}).setdefault(entry.requirement, []).append(entry.command)

    entries: list[HandleEntry] = []

    for type, feature_entries in type_feature_entries.items():

        features: list[Feature] = []

        for requirement, commands in feature_entries.items():

            feature = Feature(requirement, commands)
            features.append(feature)

        entry = HandleEntry(type, features)
        entries.append(entry)

    return entries

def tokens_patcheable(tokens: list[Token]) -> bool:

    first_symbol = find_first_token_type(tokens, TokenType.SYMBOL)

    if not first_symbol:

        return False

    if first_symbol.value != "//":

        return False

def parse_tokens(tokens: list[Token]):
    
    if not tokens:
        return

    left_hand = tokens.pop()
    operator = tokens.pop()

def patch_file_line(line: str) -> str:

    tokens = tokenize(line, ["//", "{", "}"])
    patched = ""

    if not tokens_patcheable(tokens):
        return line

    for token in tokens:

        patched += f"({token.value})"


    return patched

def patch_file(file_path: Path):
    data = ""

    with open(file_path, "r") as file:

        for line in file.readlines():

            data += patch_file_line(line)

    print(data)

def start(registry: etree.ElementTree, api: VulkanApi, file_paths: list[Path]):

    # for entry in parse_handle_entries(registry, api):

    #     print(entry.type)

    #     for feature in entry.features:

    #         print(parse_requirement(feature.requirement))
    #         print(",".join(feature.commands))
    #         print()

    for file_path in file_paths:

        patch_file(file_path)

def isolate_token_values(tokenized: list[Token]) -> list[str]:

    isolated: list[str] = []

    for token in tokenized:
        isolated.append(token.value)

    return isolated

def find_first_token_type(tokens: list[Token], target: TokenType) -> Token | None:

    for token in tokens:

        if token.type == target:

            return token

    return None

def find_limiter(line: str, limiters: list[str], index):
    
    for limiter in limiters:

        if line.startswith(limiter, index):

            return limiter

    return ""

def tokenize(line: str, limiters: list[str]) -> list[Token]:

    tokenized: list[Token] = []
    atom_buffer = ""

    index = 0

    while index < len(line):
        limiter = find_limiter(line, limiters, index)
        char = line[index]

        if limiter and atom_buffer:
            tokenized.append(Token(TokenType.ATOM, atom_buffer))
            atom_buffer = ""

        if limiter:
            tokenized.append(Token(TokenType.SYMBOL, limiter))
            index += len(limiter)
            continue

        atom_buffer += char
        index += 1

    return tokenized
        
registry = get_registry()
# source_file = Path(__file__).parent / "dok_unparched.c"
header_file = Path(__file__).parent / "test.h"

start(registry, VulkanApi.VULKAN, [header_file])