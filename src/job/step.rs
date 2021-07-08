use async_std::{
    io::{self, stdin},
    task::{self, Task}
};

pub struct Step {
    name: String,
}
impl Step {

    async fn task_name() {
        println!("[Task {:?}]", task::current().name());
    }

    pub async fn spawn_task(name: &str) -> io::Result<Self> {
        let tk = Self { name: name.into() };
        task::Builder::new()
            .name(name.to_string())
            .spawn(Self::task_name())?
            .await;
        Ok(tk)
    }

}
