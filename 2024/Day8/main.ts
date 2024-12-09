import {readFileSync} from 'fs';

let size = 50;

class Point {
    x: number = 0;
    y: number = 0;

    constructor(x: number = 0, y: number = 0) {
        this.x = x;
        this.y = y;
    }

    get locKey(): string {
        return `${this.x},${this.y}`;
    }

    get isValid(): boolean {
        return this.x >= 0 && this.x < size && this.y >= 0 && this.y < size;
    }

    static fromDifference(base: Point, xDif: number, yDif: number): Point {
        return new Point(base.x + xDif, base.y + yDif);
    }
}

function addPointToSet(set: Set<string>, point: Point): void {
    if (point.isValid && !set.has(point.locKey)) {
        set.add(point.locKey);
    }
}

function addDirectionalSteps(set: Set<string>, start: Point, xDif: number, yDif: number): void {
    let steps = 0;
    let currentPoint = new Point();

    while (true) {
        currentPoint.x = start.x + steps * xDif;
        currentPoint.y = start.y + steps * yDif;

        if (!currentPoint.isValid) {
            break;
        }

        addPointToSet(set, currentPoint);
        steps++;
    }
}

function main(): void {
    const file = readFileSync('./input.txt', 'utf-8');
    const lines = file.split('\n');

    let frequencyMap = new Map<string, Point[]>();

    for (let i = 0; i < size; i++) {
        for (let j = 0; j < size; j++) {
            const char = lines[i].charAt(j);
            if (char !== '.') {
                if (!frequencyMap.has(char)) {
                    frequencyMap.set(char, []);
                }
                frequencyMap.get(char)?.push(new Point(i, j));
            }
        }
    }

    let modelLocations = new Set<string>();
    let newModelLocations = new Set<string>();

    frequencyMap.forEach((antennaList) => {
        antennaList.forEach((first) => {
            antennaList.forEach((second) => {
                if (first.x !== second.x && first.y !== second.y) {
                    const xDif = second.x - first.x;
                    const yDif = second.y - first.y;
                    addPointToSet(modelLocations, Point.fromDifference(second, xDif, yDif));
                    addDirectionalSteps(newModelLocations, first, xDif, yDif);
                    addDirectionalSteps(newModelLocations, first, -xDif, -yDif);
                }
            });
        });
    });

    console.log("Antinode locations: " + modelLocations.size);
    console.log("Updated model locations: " + newModelLocations.size);
}

main();
