extern crate rand;

use rand::Rng;
use std::io::Write;

fn input(varname: &str, min: usize) -> usize {
	print!("\x1B[1;94mNumber of {varname}\x1B[0m: ");
	std::io::stdout().flush().unwrap();

    let mut buffer = String::new();
    std::io::stdin().read_line(&mut buffer).unwrap();

	match buffer.trim().parse::<usize>() {
        Ok(res) => {
			if res < min {
				println!("\x1B[1;91mError\x1B[0m: Should at least have {min} {varname}");
				return input(varname, min);
			}
			res
		},
		Err(err) => {
			println!("\x1B[1;91mError\x1B[0m: {err}");
			input(varname, min)
		}
	}
}

fn main() {
	let nb_ants = input("ants", 1);
	let nb_rooms = input("rooms", 2);
	let nb_links = input("links", 1);

	println!("{nb_ants}");
	for i in 0..nb_rooms {
		match i {
			0 => println!("##start"),
			1 => println!("##end"),
			_ => ()
		};
		println!("{i} 0 0");
	}
	let mut rng = rand::thread_rng();

	// TODO check if the link already exist
	for _ in 0..nb_links {
		let a: usize = rng.gen_range(0..nb_rooms);
		let mut b: usize;
		loop {
			b = rng.gen_range(0..nb_rooms);
			if a != b {
				break ;
			}
		}
		println!("{a}-{b}");
	}
}