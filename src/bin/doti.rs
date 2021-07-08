use doti::cmd;

#[async_std::main]
async fn main() -> std::io::Result<()> {
    cmd().await?;
    Ok(())
}
