from redbot.core import commands

class {cogName}(commands.Cog):
    """{cogDescription}"""

    version = 0.1

    def __init__(self, bot):
        self.bot = bot
        
    @commands.command()
    async def mycom(self, ctx):
        """This does stuff!"""
        await ctx.send("I can do stuff")