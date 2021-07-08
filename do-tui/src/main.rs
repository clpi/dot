use async_std::prelude::*;

#[async_std::main]
async fn main() -> surf::Result<()> {
    let mut res = surf::get("https://httpbin.org/get").await?;
    dbg!(res.body_string().await?);
    Ok(())
}
