print('=== Python Script Execution Demo ===')
print('Step 1: Basic arithmetic')
result = 10 + 20
print('10 + 20 =', result)

print('Step 2: Loop demonstration')
for i in range(1, 6):
    print('Loop iteration:', i)

print('Step 3: String operations')
name = 'MicroPython'
greeting = 'Hello, ' + name + '!'
print(greeting)

print('Step 4: List operations')
numbers = [1, 2, 3, 4, 5]
print('Numbers:', numbers)
total = sum(numbers)
print('Sum of numbers:', total)

print('Step 5: Function definition and call')
def calculate_square(x):
    return x * x

for num in [2, 3, 4]:
    square = calculate_square(num)
    print(f'{num} squared = {square}')

print('=== Script execution completed ===')