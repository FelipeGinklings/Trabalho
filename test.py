import re
import random
from test_classes import ParenthesisData

def organized_operation_to_string(organized_operation: tuple[str, str, str]) -> str:
    return organized_operation[0] + organized_operation[1] + organized_operation[2]


def organize_expression(expression_str: str) -> tuple[str, str, str]:
    # pattern = r"[+-]?[\da-zA-Z]+(?:[*/][+-]?[\da-zA-Z]+)+"
    pattern = r"[+-]?(?:\d+\.?\d*|[a-zA-Z]+)(?:[*/][+-]?(?:\d+\.?\d*|[a-zA-Z]+))+"
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


def get_next_letter(next_letter: str, multiplier: int = 1):
    return chr(ord("A") + (ord(next_letter) - ord("A") + 1) % 26) * multiplier


def separate_by_parenthesis(expression: str, letter: str = "A"):
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
    new_organized_expression = organize_expression(new_data.expression)
    new_data.expression = organized_operation_to_string(new_organized_expression)
    new_data.multiplications, new_data.operation, new_data.sums = new_organized_expression
    
    return new_data


def teste_func(t: str):
    t += "batata"


if __name__ == "__main__":
    STRING = generate_expression(10, True)
    ordered_operations = separate_by_parenthesis(STRING)