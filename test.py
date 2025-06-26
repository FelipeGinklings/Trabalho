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
    pattern = r"[+-]?\d+(?:[*/][+-]?\d+)+"
    multiplications: list[str] = re.findall(pattern, string)
    if multiplications.__len__() == 0:
        return "", string, ""
    sumsStr = string
    for mul in multiplications:
        sumsStr = sumsStr.replace(mul, "")
    multiplicationsStr = "".join(multiplications)
    startWithPlus = multiplicationsStr.startswith("+")
    if sumsStr == "":
        return multiplicationsStr[(startWithPlus if 1 else 0) :], "", ""
    if not (sumsStr.startswith("+") or sumsStr.startswith("-")):
        sumsStr = "+" + sumsStr
    return multiplicationsStr[(startWithPlus if 1 else 0) :], sumsStr[1:], sumsStr[0]


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


def generateExpression(size: int) -> str:
    if size <= 0:
        return ""
    operators = ["+", "-", "*", "/"]
    expression = str(random.randint(1, 999))
    for _ in range(1, size):
        expression += random.choice(operators)
        expression += str(random.randint(1, 999))
    return expression


def evaluateExpression(
    size: int,
) -> tuple[list[MiniExpression], list[MiniExpression]] | list[MiniExpression] | None:
    string = generateExpression(size)
    mulStr, sumsStr, operation = organizeExpression(string)
    evalResultMul = 0
    evalResultSum = 0
    if mulStr.__len__():
        evalResultMul: float = eval(mulStr)
    if sumsStr.__len__():
        evalResultSum: float = eval(operation + sumsStr)
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


if __name__ == "__main__":
    for size in range(2, 30):
        for _ in range(1, 100):
            evaluateExpression(size)
