import re
import random
from dataclasses import dataclass
from typing import Iterator, Union


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


def organize_expression(expression_str: str) -> tuple[str, str, str]:
    pattern = r"[+-]?[\da-zA-Z]+(?:[*/][+-]?[\da-zA-Z]+)+"
    multiplications: list[str] = re.findall(pattern, expression_str)
    if not multiplications:
        return "", "", expression_str

    sums_str = expression_str
    for mul in multiplications:
        sums_str = sums_str.replace(mul, "", 1)

    multiplications_str = "".join(multiplications)
    start_with_plus = multiplications_str.startswith("+")

    if sums_str == "":
        return multiplications_str[start_with_plus:], "", ""

    if not (sums_str.startswith("+") or sums_str.startswith("-")):
        sums_str = "+" + sums_str

    return multiplications_str[start_with_plus:], sums_str[0], sums_str[1:]


def match_to_string(match: re.Match[str], expression_str: str) -> str:
    return expression_str[match.start() : match.end()]


def add_to_the_last_one(
    last_first: str, mini_expressions_list: list[MiniExpression]
) -> None:
    if len(mini_expressions_list):
        last_operation = mini_expressions_list.pop()
        mini_expressions_list[-1].operation2 = last_operation.operation
        mini_expressions_list[-1].third = last_first
        mini_expressions_list[-1].has_extra = True


def create_mini_expression_lv2_helper(
    match: re.Match[str],
    numbers: Iterator[re.Match[str]],
    expression: str,
    mini_expressions_list: list[MiniExpression],
) -> (
    tuple[MiniExpression, re.Match[str]]
    | tuple[MiniExpression, None]
    | tuple[None, re.Match[str]]
    | tuple[None, None]
):
    new_first = match_to_string(match, expression)
    if match.end() == len(expression):
        add_to_the_last_one(new_first, mini_expressions_list)
        return None, None
    operation = expression[match.end()]
    if operation in "+-":
        add_to_the_last_one(new_first, mini_expressions_list)
        mini_expressions_list.append(MiniExpression("", operation))
        return None, None
    new_match = next(numbers)
    second = match_to_string(new_match, expression)
    return MiniExpression(new_first, operation, second), new_match


def create_mini_expressions_lv2(mul_str: str) -> list[MiniExpression] | None:
    if not len(mul_str):
        return
    mini_expressions_list: list[MiniExpression] = []
    numbers = re.finditer(r"[a-zA-Z\d]+", mul_str)
    for match in numbers:
        mini_expression, last_match = create_mini_expression_lv2_helper(
            match, numbers, mul_str, mini_expressions_list
        )
        if mini_expression == None and last_match and mul_str[last_match.end()] != "-":
            mini_expressions_list.append(MiniExpression("", mul_str[last_match.end()]))
        elif mini_expression and last_match == None:
            mini_expressions_list.append(mini_expression)
            break
        elif mini_expression and last_match:
            mini_expressions_list.append(mini_expression)
            if last_match.end() != len(mul_str):
                mini_expressions_list.append(
                    MiniExpression("", mul_str[last_match.end()])
                )
    result = ""
    if mul_str[0] == "-":
        result += "-"
    for mini_expression in mini_expressions_list:
        result += str(mini_expression)

    assert mul_str == result, f"Esperado: {mul_str}, mas obteve: {result}"

    return mini_expressions_list


def create_mini_expressions_lv1(sums_str: str) -> list[MiniExpression] | None:
    if not len(sums_str):
        return
    mini_expressions_list: list[MiniExpression] = []
    numbers = re.finditer(r"[a-zA-Z\d]+", sums_str)
    for match in numbers:
        first = match_to_string(match, sums_str)
        if match.end() == len(sums_str) and len(mini_expressions_list) > 1:
            mini_expressions_list[-1].second = first
            mini_expressions_list[-1].has_extra = True
            break
        if match.end() == len(sums_str):
            mini_expressions_list.append(MiniExpression(first))
            break
        operation = sums_str[match.end()]
        match = next(numbers)
        second = match_to_string(match, sums_str)
        mini_expressions_list.append(MiniExpression(first, operation, second))
        if match.end() != len(sums_str):
            operation = sums_str[match.end()]
            mini_expressions_list.append(MiniExpression("", operation))

    result = ""
    for mini_expression in mini_expressions_list:
        result += str(mini_expression)
    assert sums_str == result, f"Esperado: {sums_str}, mas obteve: {result}"

    return mini_expressions_list


def generate_expression(size: int, with_parenthesis: bool = False) -> str:
    if size <= 0:
        return ""
    operators = ["+", "-", "*", "/"]
    expression = str(random.randint(1, 99))
    for _ in range(1, size):
        expression += random.choice(operators)
        expression += str(random.randint(1, 99))

    if with_parenthesis and size >= 3:
        # Calculate the number of parentheses pairs based on the size
        num_parentheses = min(
            size // 2, 3
        )  # Limit to a reasonable number of parentheses
        for _ in range(num_parentheses):
            # Find positions where we can insert opening parenthesis (before numbers)
            open_positions: list[int] = []
            close_positions: list[int] = []

            # Find all number positions
            for i, char in enumerate(expression):
                if char.isdigit() and (i == 0 or not expression[i - 1].isdigit()):
                    open_positions.append(i)
                if char.isdigit() and (
                    i == len(expression) - 1 or not expression[i + 1].isdigit()
                ):
                    close_positions.append(i + 1)

            if len(open_positions) >= 2 and len(close_positions) >= 2:
                # Choose random positions for parentheses
                open_pos = random.choice(open_positions[:-1])  # Not the last number
                close_pos = random.choice(
                    [pos for pos in close_positions if pos > open_pos + 2]
                )

                if close_pos:
                    expression = (
                        expression[:open_pos]
                        + "("
                        + expression[open_pos:close_pos]
                        + ")"
                        + expression[close_pos:]
                    )
                    # Update the positions after adding parentheses
                    # This is a simplified approach, in a more robust solution,
                    # we would need to adjust the positions based on the added parentheses
                    open_positions = []
                    close_positions = []
                    for i, char in enumerate(expression):
                        if char.isdigit() and (
                            i == 0 or not expression[i - 1].isdigit()
                        ):
                            open_positions.append(i)
                        if char.isdigit() and (
                            i == len(expression) - 1 or not expression[i + 1].isdigit()
                        ):
                            close_positions.append(i + 1)

    return expression


def evaluate_expression(
    size: int,
) -> tuple[list[MiniExpression], list[MiniExpression]] | list[MiniExpression] | None:
    generated_expression = generate_expression(size)
    if eval(generated_expression) > 1e15:
        return
    mul_str, sums_str, operation = organize_expression(generated_expression)
    eval_result_mul = 0
    eval_result_sum = 0
    if len(mul_str):
        eval_result_mul: float = round(eval(mul_str), 1)
    if len(sums_str):
        eval_result_sum: float = round(eval(operation + sums_str))
    eval_result = 0
    eval_result = eval_result_mul + eval_result_sum
    eval_string: float = round(eval(generated_expression), 1)
    result_eval = round(eval_result - eval_string, 1)
    assert (
        result_eval == 0
    ), f"Esperado: 0, mas obteve: {result_eval} a string era {generated_expression} a nova string virou {mul_str+operation+sums_str}"
    print(operation)
    list_lv1 = create_mini_expressions_lv1(sums_str)
    list_lv2 = create_mini_expressions_lv2(mul_str)

    if list_lv1 and list_lv2:
        return list_lv1, list_lv2
    if list_lv1:
        return list_lv1
    if list_lv2:
        return list_lv2
    return


def get_next_letter(letter_multiply: int, next_letter: str):
    return chr(ord("A") + (ord(next_letter) - ord("A") + 1) % 26) * letter_multiply


def update_dictionary_entry(
    dict_of_letters: dict[str, str],
    next_letter: str,
    mul: str,
    op: str,
    sums: str,
):
    if next_letter in dict_of_letters.keys():
        dict_of_letters[next_letter] += mul + op + sums
    else:
        dict_of_letters[next_letter] = mul + op + sums


@dataclass
class Interval:
    start: int
    end: int

    def between(self, other: "Interval") -> bool:
        return self.start < other.start and other.end < self.end


def classify_by_parenthesis_level(expr: str):
    levels_dict: dict[int, list[str]] = {}
    current_level = 0
    current_expr: list[str] = []

    for char in expr:
        if char == "(":
            if current_expr:
                levels_dict.setdefault(current_level, []).append("".join(current_expr))
                current_expr = []
            current_level += 1
        elif char == ")":
            if current_expr:
                levels_dict.setdefault(current_level, []).append("".join(current_expr))
                current_expr = []
            current_level -= 1
        else:
            current_expr.append(char)

    # Add any remaining expression at the final level
    if current_expr:
        levels_dict.setdefault(current_level, []).append("".join(current_expr))

    return levels_dict


@dataclass
class Data:
    expression: str = ""
    has_l_operation: bool = False
    left: str = ""
    has_r_operation: bool = False
    right: str = ""
    is_operation_only: bool = False
    has_visited: bool = False


@dataclass
class Node:
    data: Data
    left: Union["Node", None]
    right: Union["Node", None]


@dataclass
class ChainedList:
    root: Node | None


def extract_parenthesis_intervals(expression_str: str):
    parenthesis_matches = re.finditer(r"[\(\)]", expression_str)
    interval_list: list[Interval] = []
    start = 0
    for match in parenthesis_matches:
        end = match.start()
        if end - start == 0:
            start = end + 1
            continue
        interval_list.append(Interval(start, end))
        start = end + 1
    if (start - len(expression_str) - 1) > 1:
        interval_list.append(Interval(start, len(expression_str) - 1))
    return interval_list


def has_operation(expression_sequence: str) -> tuple[bool, bool]:
    if len(expression_sequence) <= 1:
        return False, False
    left = expression_sequence[0]
    right = expression_sequence[-1]
    operations = "+-/*"
    if left in operations and right in operations:
        return True, True
    if left in operations:
        return True, False
    if right in operations:
        return False, True
    return False, False


def create_data(expression_sequence: str) -> Data:
    if len(expression_sequence) <= 1:
        return Data(is_operation_only=True)
    has_l_operation, has_r_operation = has_operation(expression_sequence)
    left, right = expression_sequence[0], expression_sequence[-1]
    return Data(expression_sequence, has_l_operation, left, has_r_operation, right)


def create_chained_list(expression_sequence: list[str]) -> ChainedList | None:
    new_chained_list = ChainedList(
        Node(create_data(expression_sequence[0]), None, None)
    )
    if not new_chained_list.root:
        return
    current_node = new_chained_list.root
    for expression in expression_sequence[1:-1]:
        new_node = Node(create_data(expression), None, None)
        current_node.right = new_node
        new_node.left = current_node
        current_node = new_node
    new_node = Node(create_data(expression_sequence[-1]), None, None)
    new_node.right = new_chained_list.root
    new_node.left = current_node
    new_chained_list.root.left = new_node
    return new_chained_list


def get_highest_level_value(levels_dict: dict[int, list[str]]):
    key_list = list(levels_dict.keys())
    key_list.sort()
    new_first = levels_dict[key_list[-1]][0]
    return new_first


def are_all_visited(ordered_list_of_expressions: ChainedList) -> bool:
    if (
        not ordered_list_of_expressions.root
        or not ordered_list_of_expressions.root.left
    ):
        return False
    data = ordered_list_of_expressions.root.left.data
    nav = ordered_list_of_expressions.root
    while nav and data != nav.data:
        if not nav.data.has_visited:
            return False
        nav = nav.right
    return True


def get_first_node(
    first_expression: str, chained_list: ChainedList | None
) -> Node | None:
    if not chained_list or not chained_list.root:
        return None
    nav = chained_list.root.right
    while nav and chained_list.root != nav:
        if nav.data.expression == first_expression:
            return nav
        nav = nav.right
    if chained_list.root.data.expression == first_expression:
        return chained_list.root
    return None


if __name__ == "__main__":
    # STRING = generate_expression(10, True)
    STRING = "32+61*((17+49*93-21+55)+73)*(17*65)"
    intervals = extract_parenthesis_intervals(STRING)
    levels = classify_by_parenthesis_level(STRING)
    expression_order = [STRING[interval.start : interval.end] for interval in intervals]
    new_chained_list = create_chained_list(expression_order)
    first = get_highest_level_value(levels)
    first_node = get_first_node(first, new_chained_list)

    print(first)
