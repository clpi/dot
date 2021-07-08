use async_std::{
    channel::{unbounded, Receiver, RecvError},
    task,
};
// use crate::job::Task;

#[derive(Debug, Clone)]
pub struct Queue<J> {
    id: usize,
    data: Vec<J>,
}
