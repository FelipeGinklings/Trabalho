from dataclasses import dataclass, field
from typing import Union


@dataclass
class Operation:
    operation: str = ""

    def __eq__(self, other: object) -> bool:
        if not isinstance(other, Operation):
            return False
        return self.operation == other.operation

    def __lt__(self, other: "Operation") -> bool:
        priority = {"+": 1, "-": 1, "*": 2, "/": 2}
        return priority[self.operation] < priority[other.operation]

    def __gt__(self, other: "Operation") -> bool:
        priority = {"+": 1, "-": 1, "*": 2, "/": 2}
        return priority[self.operation] > priority[other.operation]


@dataclass
class Data:
    value: str = ""
    is_operation: bool = False


@dataclass
class Node:
    data: Data
    left: Union["Node", None] = None
    right: Union["Node", None] = None


@dataclass
class ParenthesisData:
    expression: str = ""
    multiplications: str = ""
    operation: str = ""
    sums: str = ""
    next_parenthesis: dict[str, "ParenthesisData"] = field(default_factory=lambda: {})


class Tree:
    start: Node | None = None
