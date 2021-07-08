pub mod job;
pub mod util;
pub mod signal;
pub mod queue;
pub mod fs;

use async_process::{Command, Stdio};
use futures_lite::{io::BufReader, prelude::*};

pub async fn cmd() -> std::io::Result<()> {
    let mut child = Command::new("ls")
        .arg("/home/chrisp/dvsa/dito/")
        .stdout(Stdio::piped())
        .spawn()?;
    let mut lines = BufReader::new(child.stdout.take().unwrap()).lines();
    while let Some(line) = lines.next().await {
        println!("{}", line?);
    }
    Ok(())
}

#[cfg(test)]
mod tests {
    #[test]
    fn it_works() {
        assert_eq!(2 + 2, 4);
    }
}
