extern crate rand;

use rand::Rng;
use std::char::MAX;
use std::io::Write;
use std::collections::BTreeSet;

const MAX_POS: usize = 100;

fn find(u: usize, par: &mut Vec<i32>) -> usize {
	if par[u] == -1 {
		u
	} else {
		par[u] = find(par[u] as usize, par) as i32;
		par[u] as usize
	}
}

fn uni(u: usize, v: usize, par: &mut Vec<i32>) {
	let p = find(u, par);
	let q = find(v, par);
	if p != q {
		par[p] = q as i32;
	}
}

fn input(varname: &str, min: usize) -> usize {
	eprint!("\x1B[1;94mNumber of {varname}\x1B[0m: ");
	std::io::stdout().flush().unwrap();

	let mut buffer = String::new();
	std::io::stdin().read_line(&mut buffer).unwrap();

	match buffer.trim().parse::<usize>() {
		Ok(res) => {
			if res < min {
				eprintln!("\x1B[1;91mError\x1B[0m: Should at least have {min} {varname}");
				return input(varname, min);
			}
			res
		},
		Err(err) => {
			eprintln!("\x1B[1;91mError\x1B[0m: {err}");
			input(varname, min)
		}
	}
}

fn main() {
	let mut rng = rand::thread_rng();

	let nb_ants = input("ants", 1);
	let nb_rooms = input("rooms", 2);
	let nb_links = input("links", 1);

	let mut par = vec![-1; nb_rooms];

	if nb_links > nb_rooms * (nb_rooms - 1) / 2 {
		eprintln!("\x1B[1;91mError\x1B[0m: Too many links");
		return;
	}

	println!("{nb_ants}");
	for i in 0..nb_rooms {
		match i {
			0 => println!("##start"),
			1 => println!("##end"),
			_ => ()
		};
		println!("{i} {} {}", rng.gen_range(0..MAX_POS), rng.gen_range(0..MAX_POS));
	}


	let mut links: BTreeSet<(usize, usize)> = BTreeSet::new();
	// TODO check if the link already exist
	while links.len() < nb_links || find(0, &mut par) != find(1, &mut par) {
		let a: usize = rng.gen_range(0..nb_rooms);
		let mut b: usize;
		loop {
			b = rng.gen_range(0..nb_rooms);
			if links.contains(&(a, b)) || links.contains(&(b, a)) {
				break;
			}
			if a != b {
				println!("{a}-{b}");
				uni(a, b, &mut par);
				links.insert((a, b));
				break ;
			}
		}
	}
}
