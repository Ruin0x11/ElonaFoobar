Iter = {}

function Iter.rectangle_iter_step (size, pos)
   local old = pos
   pos.x = pos.x + 1
   if pos.x == size.sx then
      pos.x = size.x
      pos.y = pos.y + 1
      if pos.y == size.sy then
         return nil, nil
      end
   end

   return pos, old
end

function Iter.rectangle_iter (x, y, sx, sy)
   return Iter.rectangle_iter_step, {sx=sx, sy=sy, x=x}, {x=x, y=y}
end

return Iter
