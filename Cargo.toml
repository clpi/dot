[package]
name = "doti"
description = "Process manager and executor for dito infra"
version = "0.1.0"
edition = "2018"
default-run = "doti"

[[bin]]
name = "doti"
path = "src/bin/doti.rs"

[lib]
name = "doti"
path = "src/lib.rs"

# See more keys and their definitions at https://doc.rust-lang.org/cargo/reference/manifest.html

[dependencies]
do-core = { path = "./do-core" }
async-process = "1.1.0"
async-std = { version = "1.9.0", features=["attributes"]}
async-trait = "0.1.50"
chrono = { version = "0.4.19", features = ["serde"] }
futures-lite = "1.12.0"

[alias]
rds = "run --bin doti-server"
rd = "run --bin doti"

[workspace]
members = [
  "do-tui",
  "do-http",
  "do-rpc",
  "do-core",
  "do-core/cloud"
]
exclude = []

[profile.dev]
split-debuginfo = "unpacked"
