-- function fib(n)
--    if n == 1 or n == 2 then
--       return 1,1
--    end
--    prev, prevPrev = fib(n-1)
--    return prev+prevPrev, prev
-- end

-- print(fib(5))
-- print((fib(10)))

-- number = cAdd(2, 10)

-- print("This is lua " .. number)

-- counter = counter + GetFrameRate()

-- if counter >= 1 then
--    counter = 0
--    number = 1 / GetFrameRate()
--    print("Lua " .. number)
-- end

--SetPosition("FloorMesh", 0, 0, 0)
AddPosition("FloorMesh", -0 * GetDeltaTime(), 0 * GetDeltaTime(), 0)
--SetPosition("FloorMesh", 0,0,0)

--number = 0
number = number + GetDeltaTime()


print(number)

-- if number > 1 then
--    number = 0
--    print("SpawnEnemy")
--    CreateEnemy(0,1,0)
-- end