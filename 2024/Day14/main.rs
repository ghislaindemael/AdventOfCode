use regex::Regex;
use std::collections::HashMap;
use std::fs::File;
use std::hash::Hash;
use std::io::{BufRead, BufReader};
use std::path::Path;

struct Robot {
    pos_x: i32,
    pos_y: i32,
    v_x: i32,
    v_y: i32,
    init_x: i32,
    init_y: i32,
}

impl Robot {
    fn new(init_x: i32, init_y: i32, v_x: i32, v_y: i32) -> Robot {
        Robot {
            pos_x: init_x,
            pos_y: init_y,
            v_x,
            v_y,
            init_x,
            init_y,
        }
    }

    fn step(&mut self, width: i32, height: i32) {
        self.pos_x += self.v_x;
        self.pos_y += self.v_y;

        self.pos_x = (self.pos_x % width + width) % width;

        self.pos_y = (self.pos_y % height + height) % height;
    }

    fn get_position_at_step(&self, steps: i32, width: i32, height: i32) -> (i32, i32) {
        let mut final_x = (self.init_x + steps * self.v_x) % width;
        if final_x < 0 {
            final_x += width;
        }
        let mut final_y = (self.init_y + steps * self.v_y) % height;
        if final_y < 0 {
            final_y += height;
        }
        (final_x, final_y)
    }

    fn get_quadrant_after_steps(&self, steps: i32, width: i32, height: i32) -> i32 {
        let (x, y) = self.get_position_at_step(steps, width, height);
        get_quadrant(x, y, width, height)
    }
}
fn lines_from_file(filename: impl AsRef<Path>) -> Vec<String> {
    let file = File::open(filename).expect("No such file");
    let buf = BufReader::new(file);
    buf.lines()
        .map(|l| l.expect("Could not parse line"))
        .collect()
}

fn get_quadrant(x: i32, y: i32, w: i32, h: i32) -> i32 {
    let mid_x = (w - 1) / 2;
    let mid_y = (h - 1) / 2;
    if x > mid_x && y < mid_y {
        1
    } else if x < mid_x && y < mid_y {
        2
    } else if x < mid_x && y > mid_y {
        3
    } else if x > mid_x && y > mid_y {
        4
    } else {
        0
    }
}

fn calculate_distance(robot1: &Robot, robot2: &Robot) -> f64 {
    let (x1, y1) = (robot1.pos_x, robot1.pos_y);
    let (x2, y2) = (robot2.pos_x, robot2.pos_y);
    ((x2 - x1).pow(2) + (y2 - y1).pow(2)) as f64
}

fn print_grid(robots: &Vec<Robot>, width: i32, height: i32) {
    let mut grid = vec![vec!['.'; height as usize]; width as usize];
    for robot in robots {
        let (x, y) = (robot.pos_x, robot.pos_y);
        if x >= 0 && x < width && y >= 0 && y < height {
            grid[x as usize][y as usize] = '#';
        }
    }

    for row in grid.iter() {
        println!("{}", row.iter().collect::<String>());
    }
}

fn main() {
    let lines = lines_from_file("2024/Day14/input.txt");
    let re = Regex::new(r"p=(-?\d+),(-?\d+)\s+v=(-?\d+),(-?\d+)").unwrap();
    let mut robots_per_quadrant_map: HashMap<i32, i32> = HashMap::new();
    let width = 101;
    let height = 103;
    let steps = 100;

    let mut robots: Vec<Robot> = Vec::new();

    for line in lines {
        if let Some(caps) = re.captures(&*line) {
            let p1: i32 = caps[1].parse().unwrap();
            let p2: i32 = caps[2].parse().unwrap();
            let v1: i32 = caps[3].parse().unwrap();
            let v2: i32 = caps[4].parse().unwrap();
            robots.push(Robot::new(p1, p2, v1, v2));
        }
    }

    robots.iter().for_each(|r| {
        let quadrant = r.get_quadrant_after_steps(steps, width, height);
        if quadrant > 0 {
            *robots_per_quadrant_map.entry(quadrant).or_insert(0) += 1;
        }
    });

    let mut safety_factor = 1;
    for i in 1..=4 {
        let count = robots_per_quadrant_map.get(&i).unwrap_or(&0);
        safety_factor *= count;
        print!("Q{}: {}, ", i, count);
    }
    println!();
    println!("Safety factor after {} steps: {}", steps, safety_factor);

    // Part 2
    let mut step = 0;
    let mut closest_step = 0;
    let mut smallest_distance = f64::MAX;
    let mut total_distance: f64;
    let mut pair_count: i32;

    loop {
        total_distance = 0.0;
        pair_count = 0;

        for i in 0..robots.len() {
            for j in (i + 1)..robots.len() {
                let distance = calculate_distance(&robots[i], &robots[j]);
                total_distance += distance;
                pair_count += 1;
            }
        }

        let average_distance = total_distance / pair_count as f64;

        if average_distance < smallest_distance {
            smallest_distance = average_distance;
            closest_step = step;
        }

        if step >= 10000 {
            break;
        }

        robots
            .iter_mut()
            .for_each(|r| r.step(width, height));
        step += 1;
    }

    println!(
        "The smallest average distance occurred at step {}: {}",
        closest_step, smallest_distance
    );

    let robots_at_step: Vec<Robot> = robots
        .iter()
        .map(|robot| {
            let (x, y) = robot.get_position_at_step(closest_step, width, height);
            Robot::new(x, y, robot.v_x, robot.v_y)
        })
        .collect();

    print_grid(&robots_at_step, width, height);
}
