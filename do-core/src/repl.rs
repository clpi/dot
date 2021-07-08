
use async_std::{io::{self, prelude::*,  Write, ReadExt}, task};

pub async fn repl() -> io::Result<()> {
    let stdin = io::stdin();
    let mut stdout = io::stdout();
    let mut line = String::new();
    loop {
        let n = stdin.read_line(&mut line).await?;
        if n == 0 {
            return Ok(());
        }
        stdout.write_all(format!("OUT: {}", line).as_bytes()).await?;
        stdout.flush().await?;
        line.clear();
    }
}
