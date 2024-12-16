import * as fs from "fs";

function isWholeNumber(value: number): boolean {
    return Math.abs(value - Math.round(value)) < 1e-2;
}

function isPressNumberValid(number: number): boolean {
    return number >= 0 && number < 100 && isWholeNumber(number);
}

function isMatrixInvertible(matrix: number[][]): boolean {
    if (matrix.length !== 2 || matrix[0].length !== 2 || matrix[1].length !== 2) {
        throw new Error("Matrix must be 2x2");
    }

    let a = matrix[0][0];
    let b = matrix[0][1];
    let c = matrix[1][0];
    let d = matrix[1][1];

    return (a * d) - (b * c) !== 0;
}

function invertMatrix(matrix: number[][]): number[][] {
    if (!isMatrixInvertible(matrix)) {
        return [];
    }
    let a = matrix[0][0];
    let b = matrix[0][1];
    let c = matrix[1][0];
    let d = matrix[1][1];
    let det = (a * d) - (b * c);
    return [[d / det, -b / det], [-c / det, a / det]]
}

function multiplyMatrices(left: number[][], right: number[][]): number[][] {
    const leftRows = left.length;
    const leftCols = left[0].length;
    const rightRows = right.length;
    const rightCols = right[0].length;

    if (leftCols !== rightRows) {
        throw new Error("Invalid Dimensions");
    }

    const result: number[][] = Array.from({length: leftRows}, () => Array(rightCols).fill(0));

    for (let i = 0; i < leftRows; i++) {
        for (let j = 0; j < rightCols; j++) {
            for (let k = 0; k < leftCols; k++) {
                result[i][j] += left[i][k] * right[k][j];
            }
        }
    }

    return result;
}

function parseClawMachines(content: string): Array<{ coeffs: number[][]; prize: number[][] }> {
    const lines = content.split("\n").filter(line => line.trim() !== "");

    const configs: Array<{ coeffs: number[][]; prize: number[][] }> = [];
    const buttonRegex = /Button ([AB]): X\+(\d+), Y\+(\d+)/;
    const prizeRegex = /Prize: X=(\d+), Y=(\d+)/;

    let xA = 0, yA = 0, xB = 0, yB = 0;

    for (const line of lines) {
        let match: RegExpExecArray | null;

        if ((match = buttonRegex.exec(line))) {
            const button = match[1];
            const xCoeff = parseInt(match[2]);
            const yCoeff = parseInt(match[3]);

            if (button === "A") {
                xA = xCoeff;
                yA = yCoeff;
            } else if (button === "B") {
                xB = xCoeff;
                yB = yCoeff;
            }
        } else if ((match = prizeRegex.exec(line))) {
            const xPrize = parseInt(match[1]);
            const yPrize = parseInt(match[2]);

            if (!xA && !yA && !xB && !yB) {
                throw new Error("Prize without buttons is not valid");
            }

            const coeffs = [
                [xA, xB],
                [yA, yB],
            ];
            const prize = [[xPrize], [yPrize]];

            configs.push({coeffs, prize});

            xA = yA = xB = yB = 0;
        }
    }

    return configs;
}


function main() {

    const content = fs.readFileSync("input.txt", "utf8");
    const machines = parseClawMachines(content);

    let minSimplePrice = 0;
    let minExpensivePrice = 0;

    machines.forEach((machine) => {

        const {coeffs, prize} = machine;

        const [aSimplePresses, bSimplePresses] = multiplyMatrices(invertMatrix(coeffs), prize).map(row => row[0]);
        if (isPressNumberValid(aSimplePresses) && isPressNumberValid(bSimplePresses)) {
            minSimplePrice += 3 * aSimplePresses;
            minSimplePrice += bSimplePresses;
        }

        const bigPrize = prize.map((row) => [row[0] + 10000000000000]);

        const [aPresses, bPresses] = multiplyMatrices(invertMatrix(coeffs), bigPrize).map(row => row[0]);
        if (aPresses >= 0 && isWholeNumber(aPresses) && bPresses >= 0 && isWholeNumber(bPresses)) {
            minExpensivePrice += 3 * aPresses;
            minExpensivePrice += bPresses;
        }

    });

    console.log("Minimum simple machine price: " + minSimplePrice);
    console.log("Minimum expensive machine price: " + minExpensivePrice);
}

main();