
import click

# @click.command()
# @click.option("--count", default=1, help="Number of greetings.")
# @click.option("--name", prompt="Your name",
#               help="The person to greet.")
@click.group()
@click.version_option(version='1.0.0')
def greet():
    pass


@greet.command()
@click.argument('name')  # add the name argument
@click.option('--excited', is_flag=True)
@click.option('--greeting', default='Hello', help='word to use for the greeting')
def hello(**kwargs):
    out = f"{kwargs['greeting']} {kwargs['name']}"
    if kwargs['excited']: print(f"{out}!!")
    else:
        print(f"{out} wut du...")


@greet.command()
@click.argument('name')  # add the name argument
@click.option('--verbose', default=True)
def goodbye(**kwargs):
    print('Goodbye, {0}!'.format(kwargs['name']))
    pass

if __name__ == '__main__':
    greet()
