import { readFileSync } from 'fs';

function main(): void {

    const file = readFileSync('./input.txt', 'utf-8');
    const digitPattern = /\d{1,3}/g;

    const allMulsPattern = /mul\(\d{1,3},\d{1,3}\)/g;
    const allMuls = file.match(allMulsPattern);

    let totalSum = 0;

    allMuls.forEach(item => {
        const digits = item.match(digitPattern);
        totalSum += parseInt(digits[0]) * parseInt(digits[1]);
    })

    console.log("Total sum: ", totalSum);

    const activeMulsPattern = /(mul\(\d{1,3},\d{1,3}\)|do\(\)|don't\(\))/g;
    const activeMuls = file.match(activeMulsPattern);

    let activeSum = 0;
    let active = true;

    activeMuls.forEach(item => {
        if(item == "do()") {
            active = true;
        } else if (item == "don't()") {
            active = false;
        } else if(active){
            const digits = item.match(digitPattern);
            activeSum += parseInt(digits[0]) * parseInt(digits[1]);
        }
    })

    console.log("Sum fo active muls: ", activeSum);


}


main();