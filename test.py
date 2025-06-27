import re
import random
from dataclasses import dataclass
from typing import Iterator


@dataclass
class MiniExpression:
    first: str = ""
    operation: str = ""
    second: str = ""
    hasExtra: bool = False
    operation2: str = ""
    third: str = ""

    def __str__(self):
        result = self.first + self.operation + self.second
        if self.hasExtra:
            result += self.operation2 + self.third
        return result


def organizeExpression(string: str) -> tuple[str, str, str]:
    pattern = r"[+-]?[\da-zA-Z]+(?:[*/][+-]?[\da-zA-Z]+)+"
    multiplications: list[str] = re.findall(pattern, string)
    if not multiplications:
        return "", "", string

    sumsStr = string
    for mul in multiplications:
        sumsStr = sumsStr.replace(mul, "", 1)

    multiplicationsStr = "".join(multiplications)
    startWithPlus = multiplicationsStr.startswith("+")

    if sumsStr == "":
        return multiplicationsStr[startWithPlus:], "", ""

    if not (sumsStr.startswith("+") or sumsStr.startswith("-")):
        sumsStr = "+" + sumsStr

    return multiplicationsStr[startWithPlus:], sumsStr[0], sumsStr[1:]


# def organizeExpression(
#     string: str, withLetters: bool = False, withNumbers: bool = False
# ) -> tuple[str, str, str]:
#     pattern = r"[+-]?\d+(?:[*/][+-]?\d+)+"
#     if withLetters:
#         pattern = r"[+-]?[a-zA-Z]+(?:[*/][+-]?[a-zA-Z]+)+"
#     multiplications: list[str] = re.findall(pattern, string)
#     if multiplications.__len__() == 0:
#         return "", string, ""
#     sumsStr = string
#     for mul in multiplications:
#         sumsStr = sumsStr.replace(mul, "", 1)
#     multiplicationsStr = "".join(multiplications)
#     startWithPlus = multiplicationsStr.startswith("+")
#     if sumsStr == "":
#         return multiplicationsStr[(startWithPlus if 1 else 0) :], "", ""
#     if not (sumsStr.startswith("+") or sumsStr.startswith("-")):
#         sumsStr = "+" + sumsStr
#     return multiplicationsStr[(startWithPlus if 1 else 0) :], sumsStr[1:], sumsStr[0]


def matchToString(match: re.Match[str], string: str) -> str:
    return string[match.start() : match.end()]


def addToTheLastOne(first: str, miniExpressionsList: list[MiniExpression]) -> None:
    lastOperation = miniExpressionsList.pop()
    miniExpressionsList[-1].operation2 = lastOperation.operation
    miniExpressionsList[-1].third = first
    miniExpressionsList[-1].hasExtra = True


def createMiniExpression(
    match: re.Match[str],
    numbers: Iterator[re.Match[str]],
    expression: str,
    miniExpressionsList: list[MiniExpression],
) -> (
    tuple[MiniExpression, re.Match[str]]
    | tuple[MiniExpression, None]
    | tuple[None, re.Match[str]]
    | tuple[None, None]
):
    first = matchToString(match, expression)
    if match.end() == expression.__len__():
        addToTheLastOne(first, miniExpressionsList)
        return None, None
    operation = expression[match.end()]
    if operation in "+-":
        addToTheLastOne(first, miniExpressionsList)
        miniExpressionsList.append(MiniExpression("", operation))
        return None, None
    newMatch = next(numbers)
    second = matchToString(newMatch, expression)
    return MiniExpression(first, operation, second), newMatch


def createMiniExpressionsLv2(mulStr: str) -> list[MiniExpression] | None:
    if not mulStr.__len__():
        return
    miniExpressionsList: list[MiniExpression] = []
    numbers = re.finditer(r"\b\d+\b", mulStr)
    for match in numbers:
        miniExpression, lastMatch = createMiniExpression(
            match, numbers, mulStr, miniExpressionsList
        )
        if miniExpression == None and lastMatch and mulStr[lastMatch.end()] != "-":
            miniExpressionsList.append(MiniExpression("", mulStr[lastMatch.end()]))
        elif miniExpression and lastMatch == None:
            miniExpressionsList.append(miniExpression)
            break
        elif miniExpression and lastMatch:
            miniExpressionsList.append(miniExpression)
            if lastMatch.end() != mulStr.__len__():
                miniExpressionsList.append(MiniExpression("", mulStr[lastMatch.end()]))
    result = ""
    if mulStr[0] == "-":
        result += "-"
    for miniExpression in miniExpressionsList:
        result += miniExpression.__str__()

    assert mulStr == result, f"Esperado: {mulStr}, mas obteve: {result}"

    return miniExpressionsList


def createMiniExpressionsLv1(sumsStr: str) -> list[MiniExpression] | None:
    if not sumsStr.__len__():
        return
    miniExpressionsList: list[MiniExpression] = []
    numbers = re.finditer(r"\b\d+\b", sumsStr)
    for match in numbers:
        first = matchToString(match, sumsStr)
        if match.end() == sumsStr.__len__() and miniExpressionsList.__len__() > 1:
            miniExpressionsList[-1].second = first
            miniExpressionsList[-1].hasExtra = True
            break
        elif match.end() == sumsStr.__len__():
            miniExpressionsList.append(MiniExpression(first))
            break
        operation = sumsStr[match.end()]
        match = next(numbers)
        second = matchToString(match, sumsStr)
        miniExpressionsList.append(MiniExpression(first, operation, second))
        if match.end() != sumsStr.__len__():
            operation = sumsStr[match.end()]
            miniExpressionsList.append(MiniExpression("", operation))

    result = ""
    for miniExpression in miniExpressionsList:
        result += miniExpression.__str__()
    assert sumsStr == result, f"Esperado: {sumsStr}, mas obteve: {result}"

    return miniExpressionsList


def generateExpression(size: int, withParenthesis: bool = False) -> str:
    if size <= 0:
        return ""
    operators = ["+", "-", "*", "/"]
    expression = str(random.randint(1, 99))
    for _ in range(1, size):
        expression += random.choice(operators)
        expression += str(random.randint(1, 99))

    if withParenthesis and size >= 3:
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


def evaluateExpression(
    size: int,
) -> tuple[list[MiniExpression], list[MiniExpression]] | list[MiniExpression] | None:
    string = generateExpression(size)
    if eval(string) > 1e15:
        return
    mulStr, sumsStr, operation = organizeExpression(string)
    evalResultMul = 0
    evalResultSum = 0
    if mulStr.__len__():
        evalResultMul: float = round(eval(mulStr), 1)
    if sumsStr.__len__():
        evalResultSum: float = round(eval(operation + sumsStr))
    evalResult = 0
    evalResult = evalResultMul + evalResultSum
    evalString: float = round(eval(string), 1)
    resultEval = round(evalResult - evalString, 1)
    assert (
        resultEval == 0
    ), f"Esperado: 0, mas obteve: {resultEval} a string era {string} a nova string virou {mulStr+operation+sumsStr}"
    print(operation)
    listLv1 = createMiniExpressionsLv1(sumsStr)
    listLv2 = createMiniExpressionsLv2(mulStr)

    if listLv1 and listLv2:
        return listLv1, listLv2
    elif listLv1:
        return listLv1
    elif listLv2:
        return listLv2
    return


def getNextLetter(letterMultiply: int, nextLetter: str):
    return chr(ord("A") + (ord(nextLetter) - ord("A") + 1) % 26) * letterMultiply


def organizeParenthesis(
    string: str,
    dictOfLetter: dict[str, str],
    lastLetter: str = "A",
    inside: int = 0,
    letterMultiply: int = 1,
) -> tuple[None, None] | tuple[int, str]:
    if lastLetter == "Z":
        letterMultiply += 1

    operation = ""
    nextLetter = lastLetter
    isInside = inside > 0
    out = 0
    newString = ""
    returnLetter = lastLetter
    char = ""
    for index, _ in enumerate(string):
        if index + out < string.__len__():
            char = string[index + out]
        else:
            if not operation:
                break
            first = ""
            last = ""
            if operation[0] in "+-*/":
                first = operation[0]
                operation = operation[1:]
            if operation[-1] in "+-*/":
                last = operation[-1]
                operation = operation[:-1]
            mul, op, sums = organizeExpression(operation)
            dictOfLetter[nextLetter] = mul + op + sums
            newString += first + nextLetter + last
            break
        if char not in "()":
            operation += char
        elif isInside and char == "(":
            nextLetter = getNextLetter(letterMultiply, nextLetter)
            if string[index + out - 1] in "+-/*":
                operation = operation + nextLetter
            inside += 1
            newOut, returnLetterOut = organizeParenthesis(
                string[index + 1 + out :], dictOfLetter, nextLetter, inside
            )
            if newOut and returnLetterOut:
                inside -= 1
                isInside = inside > 0
                out += newOut
                returnLetter = returnLetterOut

        elif isInside and char == ")":
            mul, op, sums = organizeExpression(operation)
            dictOfLetter[lastLetter] = mul + op + sums
            return index + out + 1, returnLetter
        elif not isInside and char == "(" and index and operation.__len__() != 1:
            first = ""
            last = ""
            if operation[0] in "+-*/":
                first = operation[0]
                operation = operation[1:]
            if operation[-1] in "+-*/":
                last = operation[-1]
                operation = operation[:-1]
            mul, op, sums = organizeExpression(operation)
            dictOfLetter[nextLetter] = mul + op + sums
            newString += first + nextLetter + last
            nextLetter = getNextLetter(letterMultiply, nextLetter)
            inside += 1
            newOut, returnLetterOut = organizeParenthesis(
                string[index + out + 1 :], dictOfLetter, nextLetter, inside
            )
            newString += nextLetter
            if returnLetterOut:
                nextLetter = getNextLetter(letterMultiply, returnLetterOut)
            operation = ""
            if newOut:
                inside -= 1
                isInside = inside > 0
                out += newOut
        elif not index and operation.__len__() != 1:
            nextLetter = getNextLetter(letterMultiply, nextLetter)
            newString += nextLetter
            inside += 1
            newOut, returnLetterOut = organizeParenthesis(
                string[index + out + 1 :], dictOfLetter, nextLetter, inside
            )
            if returnLetterOut:
                nextLetter = getNextLetter(letterMultiply, nextLetter)
            operation = ""
            if newOut:
                inside -= 1
                isInside = inside > 0
                out += newOut
        elif operation.__len__() == 1:
            nextLetter = getNextLetter(letterMultiply, nextLetter)
            inside += 1
            newOut, returnLetterOut = organizeParenthesis(
                string[index + out + 1 :], dictOfLetter, nextLetter, inside
            )
            newString += operation + nextLetter
            if returnLetterOut:
                nextLetter = getNextLetter(letterMultiply, nextLetter)
            operation = ""
            if newOut:
                inside -= 1
                isInside = inside > 0
                out += newOut

    if not isInside:
        mul, op, sums = organizeExpression(newString)
        dictOfLetter["string"] = mul + op + sums
    return None, None


if __name__ == "__main__":
    string = generateExpression(10, True)
    dictOfLetters: dict[str, str] = {}
    organizeParenthesis(string, dictOfLetters)
    print()
