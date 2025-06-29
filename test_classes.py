from dataclasses import dataclass, field
from typing import Union


@dataclass
class MiniExpression:
    first: str = ""
    operation: str = ""
    second: str = ""
    has_extra: bool = False
    operation2: str = ""
    third: str = ""

    def __str__(self):
        result = self.first + self.operation + self.second
        if self.has_extra:
            result += self.operation2 + self.third
        return result


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
    operation: Operation = field(default_factory=Operation)
    is_operation: bool = False


@dataclass
class Node:
    data: Data
    left: Union["Node", None] = None
    right: Union["Node", None] = None


@dataclass
class ParenthesisData:
    expression: str = ""
    mini_expression: MiniExpression = field(default_factory=MiniExpression)
    next_parenthesis: dict[str, "ParenthesisData"] = field(default_factory=lambda: {})


class Tree:
    start: Node | None = None
