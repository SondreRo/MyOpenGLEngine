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

-- mynumber, number2 = cAdd(1,10)

-- --SetPosition("FloorMesh", 0, 0, 0)
-- AddPosition("BoxMesh", -0 * GetDeltaTime(), 0 * GetDeltaTime(), 0)

-- myvec = {1,5,2}
-- SetPosition("FloorMesh", 0,0,0, myvec)


val1 = cAdd(10,50, 25)
	
--AddPosition("FloorMesh",-5 * GetDeltaTime(), 0, 0)
-- --number = 0
-- number = number + GetDeltaTime()


-- --print(number)

if number == nil then
	number = 0
else
    number = number + GetDeltaTime() 
end




if number > 3 then
    number = 0
    print("SpawnEnemy")
    CreateEnemy(0,1,0)
end