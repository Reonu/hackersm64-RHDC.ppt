def multiple(m, n):
    return True if m % n == 0 else False
    
while True:
    width = int(input("Enter desired width"))
    
    if (multiple(width,16) == False):
        width = ((width + 16 - 1) // 16) * 16
        
    height = 8192 // width
    
    print("width:" + str(width) + " height:" + str(height))