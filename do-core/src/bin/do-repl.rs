use do_core::repl::repl;

#[async_std::main]
async fn main() -> async_std::io::Result<()> {
    repl().await?;
    Ok(())
}
