local function fail_if_missing(var, msg)
    if not var then
        error(msg or "Missing value", 3)
    end
    return false
end

local function is_valid_id(event_id)
    if not (type(event_id) == "string") then
        error("Invalid Event Id, Must be string. Passed in "..event_id, 3)
    end
end

Event = { --luacheck: allow defined top
    _registry = {},
}

function Event.register(event_ids, handler)
    fail_if_missing(event_ids, "missing event_ids argument")

    event_ids = (type(event_ids) == "table" and event_ids) or {event_ids}

    for _, event_id in pairs(event_ids) do
        is_valid_id(event_id)

        if handler == nil then
            Event._registry[event_id] = nil
            Registry.on_event(event_id, nil)
        else
            if not Event._registry[event_id] then
                Event._registry[event_id] = {}

                Registry.on_event(event_id, Event.dispatch)
            end
            --If the handler is already registered for this event: remove and insert it to the end.
            local _, reg_index = table.find(Event._registry[event_id], function(v) return v == handler end)
            if reg_index then
                table.remove(Event._registry[event_id], reg_index)
                log("Same handler already registered for event "..event_id..", reording it to the bottom")
            end
            table.insert(Event._registry[event_id], handler)
        end
    end
    return Event
end

function Event.dispatch()
        if _registry then
            for idx, handler in ipairs(_registry) do
                pcall(handler)
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
                Registry.on_event(event_id, nil)
            end
        end
    end
    return Event
end

return Event
