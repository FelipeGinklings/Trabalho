import re
import random
from typing import Iterator
from test_classes import MiniExpression, ParenthesisData


def organize_expression(expression_str: str) -> str:
    pattern = r"[+-]?[\da-zA-Z]+(?:[*/][+-]?[\da-zA-Z]+)+"
    multiplications: list[str] = re.findall(pattern, expression_str)
    if not multiplications:
        return expression_str

    sums_str = expression_str
    for mul in multiplications:
        sums_str = sums_str.replace(mul, "", 1)

    multiplications_str = "".join(multiplications)
    start_with_plus = multiplications_str.startswith("+")
    if sums_str == "":
        return multiplications_str[start_with_plus:]

    if not (sums_str.startswith("+") or sums_str.startswith("-")):
        sums_str = "+" + sums_str

    return multiplications_str[start_with_plus:] + sums_str


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
    first = match_to_string(match, expression)
    if match.end() == len(expression):
        add_to_the_last_one(first, mini_expressions_list)
        return None, None
    operation = expression[match.end()]
    if operation in "+-":
        add_to_the_last_one(first, mini_expressions_list)
        mini_expressions_list.append(MiniExpression("", operation))
        return None, None
    new_match = next(numbers)
    second = match_to_string(new_match, expression)
    return MiniExpression(first, operation, second), new_match


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


def get_next_letter(next_letter: str, multiplier: int = 1):
    return chr(ord("A") + (ord(next_letter) - ord("A") + 1) % 26) * multiplier


def separate_by_parenthesis(expression: str, letter: str = "A"):
    # has_parenthesis = re.findall(r"\(", expression)
    # if not has_parenthesis:
    #     return
    current_expression: str = ""
    new_data = ParenthesisData()
    level = 0
    next_letter = letter
    for index, char in enumerate(expression):
        if char == "(":
            if current_expression and level == 0:
                new_data.expression += current_expression + next_letter
                current_expression = ""
                level += 1
                continue
            level += 1
            if not index:
                new_data.expression += letter
                continue
        elif char == ")":
            if current_expression and level == 1:
                next_parenthesis = separate_by_parenthesis(current_expression)
                new_data.next_parenthesis[next_letter] = next_parenthesis
                next_letter = get_next_letter(next_letter)
                current_expression = ""
                level -= 1
                continue
            level -= 1
        current_expression += char
    if current_expression:
        new_data.expression += current_expression
    new_data.expression = organize_expression(new_data.expression)
    return new_data


if __name__ == "__main__":
    # STRING = generate_expression(10, True)
    STRING = "32+61*((17+49*93-21+55)+73)*(17*65)/(20-65)"
    new_levels = separate_by_parenthesis(STRING)
