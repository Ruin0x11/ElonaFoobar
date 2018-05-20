# elomod
Welcome traveler! This is the documentation for the ElonaFoobar mod API.

Before proceeding, **please note** that the API will almost certainly undergo substantial changes before it is stabilized. If you write anything with the API, expect that it **will** break sometime in the future, until all the serious design/implementation issues and bugs have been worked out.

## Testing it out
You can run a script at startup by adding a parameter to your `config.json`. Copy the `life.lua` example into your `data/script` folder, and add this line to your `config.json`:

```
"startup_script": "life.lua"
```

Starting the game will place you in a script testing map, isolated from your other saves. There you can see the script in action. The `life.lua` script will run Conway's Game of Life by setting tiles in the map. Let's break down how it works.

### Module definitions

```
local Iter = Elona.Iter
local Map = Elona.Map
local Enums = Elona.Defines.Enums
```

At the top, we have some defines. All modules related to the core API can be found under the `Elona` table. The convention for accessing them is to bind a local variable with the name of the module and use it elsewhere in the file to reduce typing.

```
local Storage = Elona.Registry.Data["script"]
```

This last module is important: it is a global data storage registry for data that needs to persist on save and load. This is how the state of the life simulation is stored on top of the standard map data. You'll be able to store data for items, characters, maps, and globally. The intention (for the time being) is that data stored in the registry for your mod will be transparently serialized and bundled alongside your game save. The benefits of this are multiple: you'll be able to extend the game without modifying the original save file format, and the details of saving and loading your custom data will be taken of for you.

The name `"script"` is the name of the storage for the startup script. If you create a mod, the registry will automatically create storage located under your mod's name.

### Map initialization

```
local function my_map_init(w, h)
   grid = {}
   for i = 1, w do
      grid[i] = {}

      for j = 1, h do
         grid[i][j] = Elona.Rand.rnd(2)
      end
   end
   return {grid=grid} -- Now you can use Storage.Map.grid
end
```

This function will set up our script's storage when the map loads. It creates a new 2D table called `grid` and sets every cell inside it to 0 or 1. Where is this used? At the bottom of `life.lua`, we can see this:

```
Elona.Registry.register_map_init(my_map_init)
```
This will register a new initialization function for our custom map data. Whenever a new map is generated from scratch, this function will be called to add your mod's data to its own base data. If you visit an existing map, its data will be loaded from your savefile instead. This initialization function must return a table describing the data that you want to store.

### Logic

```
local function evolve(cell)
-- ...
end
```

This function runs the life simulation. We won't go into the details, but if you're interested you can see [this article](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life).

```
local function run_life()
   local grid = Storage.Map.grid
   for y = 1, Map.width() do
      for x = 1, Map.height() do
         if Storage.Map.grid[x][y] == 1 and Map.can_access(x, y) then
            Map.set_tile(x, y, Enums.TileKind.Wall)
            Map.set_tile_memory(x, y, Enums.TileKind.Wall)
         else
            Map.set_tile(x, y, Enums.TileKind.Room)
            Map.set_tile_memory(x, y, Enums.TileKind.Room)
         end
      end
   end
   Storage.Map.grid = evolve(grid)
end
```

This function updates the game map based on the data we stored. Let's go into detail what it does.

```
   local grid = Storage.Map.grid
```

Here we retrieve the grid data that is kept within our mod's data storage. If you recall, our map initialization function returned this:

```
   local grid = {}
   -- ...
   return {grid=grid} -- Now you can use Storage.Map.grid
```

Since we set the value of `grid` in the table inside the initialization function, we can now use it inside `Storage` after the map loads.


```
   for y = 1, Map.width() do
      for x = 1, Map.height() do
      -- ...
      end
   end
```

Next we iterate over every x-y pair in the map. Since there is only ever one map loaded at a time, we can call `Map.width` and `Map.height` without passing in anything.

```
         if Storage.Map.grid[x][y] == 1 and Map.can_access(x, y) then
            Map.set_tile(x, y, Enums.TileKind.Wall)
            Map.set_tile_memory(x, y, Enums.TileKind.Wall)
         else
            Map.set_tile(x, y, Enums.TileKind.Room)
            Map.set_tile_memory(x, y, Enums.TileKind.Room)
         end
```

Here is where we make use of the `Map` module to modify the map. You can read the documentation for `Map.can_access` and `Map.set_tile` elsewhere in the docs. Essentially, if the simulation reports a cell with value 1, we set that square to a wall tile, else to a floor tile. We also make sure to set the player's memory of that tile so they can see it even if it's out of field of view.

We also use the enum type `TileKind` here. Some functions take enums to denote one of several different states an object can be in, like the curse state of an object (`Blessed`, `None`, `Cursed`, or `Doomed`).

### Events

```
Elona.Registry.register_map_init(my_map_init)
Elona.Event.register(Elona.Defines.EventKind.initialized_map, run_life)
Elona.Event.register(Elona.Defines.EventKind.all_turns_finished, run_life)
```

Finally, this is the most critical part of the script: hooking it up so it can be ran in response to in-game events. We've already covered the `register_map_init` function, but there is another important function to go over: `Event.register`. This will run a function you provide when an in-game event is fired. In this case, we want to run our function `run_life` when the map is first loaded, and every time all characters in the map have finished moving. This is what the calls to `Event.register` are doing. There are various kinds of events you can listen for, and they are listed in the documentation for `Defines.EventKind`.

That's all for this tutorial. Go forth and find game-breaking bugs! (and preferably report them to us)
