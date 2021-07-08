use do_core::fs::{cat, ls};
use do_http::server::{tcp::TcpServer, udp};

#[async_std::main]
async fn main() -> std::io::Result<()> {
    ls("/home/chrisp/wiki/").await?;
    cat("/home/chrisp/wiki/index.md").await?;
    TcpServer::new("0.0.0.0:8080").await
        .run().await?;
    Ok(())

}
