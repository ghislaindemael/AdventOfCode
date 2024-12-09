use std::fs::File;
use std::io::{BufRead, BufReader};
use std::path::Path;

fn lines_from_file(filename: impl AsRef<Path>) -> Vec<String> {
    let file = File::open(filename).expect("No such file");
    let buf = BufReader::new(file);
    buf.lines()
        .map(|l| l.expect("Could not parse line"))
        .collect()
}

fn get_calibration_if_valid(line: String, concat: bool) -> Option<i64> {
    let mut parts = line.split(':');
    let identifier: i64 = parts.next()?.trim().parse().ok()?;
    let numbers_part = parts.next()?.trim();
    let numbers: Vec<i64> = numbers_part
        .split_whitespace()
        .filter_map(|num| num.parse().ok())
        .collect();

    if numbers.is_empty() {
        return None;
    }

    let mut operators = vec!['+', '*'];
    if concat {
        operators.push('|')
    }
    let operator_combinations = generate_operator_combinations(numbers.len() - 1, &operators);

    for ops in operator_combinations {
        if evaluate_expression(&numbers, &ops) == identifier {
            return Some(identifier);
        }
    }

    None
}

fn generate_operator_combinations(length: usize, operators: &[char]) -> Vec<Vec<char>> {
    if length == 0 {
        return vec![vec![]];
    }

    let mut results = Vec::new();
    for op in operators {
        for mut sub_combination in generate_operator_combinations(length - 1, operators) {
            sub_combination.insert(0, *op);
            results.push(sub_combination);
        }
    }
    results
}

fn evaluate_expression(numbers: &[i64], operators: &[char]) -> i64 {
    let mut result = numbers[0].to_string();
    for (i, &op) in operators.iter().enumerate() {
        if let Some(next_number) = numbers.get(i + 1) {
            match op {
                '+' => {
                    let sum = result.parse::<i64>().unwrap_or(0) + next_number;
                    result = sum.to_string();
                }
                '*' => {
                    let product = result.parse::<i64>().unwrap_or(1) * next_number;
                    result = product.to_string();
                }
                '|' => result.push_str(&next_number.to_string()),
                _ => {}
            }
        }
    }
    result.parse::<i64>().unwrap_or(0)
}

fn main() {
    let lines = lines_from_file("2024/Day7/input.txt");
    let mut valid_ops: i64 = 0;
    let mut valid_ops_with_concat: i64 = 0;

    for line in lines {
        if let Some(value) = get_calibration_if_valid(line.clone(), false) {
            valid_ops += value;
        }
        if let Some(value) = get_calibration_if_valid(line, true) {
            valid_ops_with_concat += value;
        }
    }
    println!("Sum of valid calibrations: {}", valid_ops);
    println!(
        "Sum of valid calibrations with concatenation: {}",
        valid_ops_with_concat
    );
}
