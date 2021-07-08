use async_std::{io, prelude::*, fs::{self, File}, task};

const LEN: usize = 16 * 1024; // 16 Kb

pub async fn ls(path: &str) -> io::Result<()> {
    task::block_on(async {
        let mut dir = fs::read_dir(&path).await?;
        while let Some(res) = dir.next().await {
            let entry = res?;
            println!("{}", entry.file_name().to_string_lossy());
        }
        Ok(())
    })
}
pub async fn cat(path: &str) -> io::Result<()> {
    let mut file = File::open(&path).await?;
    let mut stdout = io::stdout();
    let mut buf = vec![0u8; LEN];
    loop {
        let n = file.read(&mut buf).await?;
        if n == 0 {
            stdout.flush().await?;
            return Ok(());
        }
        stdout.write_all(&buf[..n]).await?;
    }
}
