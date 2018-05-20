require "mods/core/table"

local function fail_if_missing(var, msg)
    if not var then
        error(msg or "Missing value", 3)
    end
    return false
end

local Event = { --luacheck: allow defined top
    _registry = {},
}

local function is_valid_id(event_id)
    if not (type(event_id) == "string") then
        error("Invalid Event Id, Must be string. Passed in "..event_id, 3)
    end
end

function Event.register(event_ids, handler)
    fail_if_missing(event_ids, "missing event_ids argument")

    event_ids = (type(event_ids) == "table" and event_ids) or {event_ids}

    for _, event_id in pairs(event_ids) do
        is_valid_id(event_id)

        if handler == nil then
           Elona.Debug.log("Handler for " .. event_id .. " was nil, setting to nil")
            Event._registry[event_id] = nil
            Elona.Registry.on_event(event_id, nil)
        else
           Elona.Debug.log("Get handler for " .. event_id .. " ")
            if not Event._registry[event_id] then
               Elona.Debug.log("New handler")
                Event._registry[event_id] = {}

                Elona.Registry.on_event(event_id, Event.dispatch)
            end
            --If the handler is already registered for this event: remove and insert it to the end.
            local _, reg_index = table.find(Event._registry[event_id], function(v) return v == handler end)
            if reg_index then
               Elona.Debug.log("Handler exists")
                table.remove(Event._registry[event_id], reg_index)
                Elona.Debug.log("Same handler already registered for event "..event_id..", reording it to the bottom")
            end
            table.insert(Event._registry[event_id], handler)
        end
    end
    return Event
end

function Event.dispatch(event_id, args)
   local _registry = Event._registry[event_id]
   if _registry then
      for idx, handler in ipairs(_registry) do
         Elona.Debug.log("Handler for " .. event_id .. ": " .. idx)
         local status, err = xpcall(handler, function(a) return debug.traceback(a) end, args)
         if not status then
            error("Error dispatching " .. event_id .. ": " .. Elona.Debug.inspect(err))
         end
      end
   end
end

function Event.remove(event_ids, handler)
    fail_if_missing(event_ids, "missing event_ids argument")
    fail_if_missing(handler, "missing handler argument")

    event_ids = (type(event_ids) == "table" and event_ids) or {event_ids}

    for _, event_id in pairs(event_ids) do
        is_valid_id(event_id)

        if Event._registry[event_id] then
            for i = #Event._registry[event_id], 1, -1 do
                if Event._registry[event_id][i] == handler then
                    table.remove(Event._registry[event_id], i)
                end
            end
            if table.size(Event._registry[event_id]) == 0 then
                Event._registry[event_id] = nil
                Elona.Registry.on_event(event_id, nil)
            end
        end
    end
    return Event
end

function Event.clear_all()
   for event_id in pairs (Event._registry) do
      Event._registry[event_id] = nil
   end
end

Elona.Event = Event