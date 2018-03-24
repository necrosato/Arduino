import socket

class OthelloServer:

    def __init__(self, game):
        html_head = """<!DOCTYPE html>
        <html>
            <head> <title>ESP8266 Micropython Othello</title> </head>
            <body> <h1>ESP8266 Micropython Othello</h1>
        """
        t2_head = '<table border="1">'
        t2_foot = '</table>'
        html_foot = """
            </body>
        </html>
        """

        addr = socket.getaddrinfo('0.0.0.0', 80)[0][-1]

        s = socket.socket()
        s.bind(addr)
        s.listen(1)

        print('listening on', addr)

        while True:
            cl, addr = s.accept()
            print('client connected from', addr)

            cl_file = cl.makefile('rwb', 0)
            while True:
                line = cl_file.readline()
                print(str(line))
                if not line or line == b'\r\n':
                    break
                elif line[0:13] == b'GET /maketurn':
                    ni = int(line[13]) - 48
                    nj = int(line[14]) - 48
                    if ni >= 0 and ni < 8 and nj >= 0 and nj < 8:
                        game.makeTurnWeb(ni, nj)

            rows = []
            for i in range(8):
                row = '<tr>'
                for j in range(8):
                    row += '<td><form name="'+str(i)+str(j)+'" action="/maketurn'+str(i)+str(j)+'"><button type=submit" style="background-color: '
                    if (game.board[i][j] == game.black):
                        row += '#555555;"> b '
                    elif (game.board[i][j] == game.white):
                        row += '#FFFFFF;"> w '
                    else:
                        row += '#e7e7e7;"> _ '
                    row += '</button></form></td>'
                row += '</tr>\n'
                rows.append(row)

            t1 = '<tr><td>' + str(game.numBlack) + '</td><td>' + str(game.numWhite) + '</td><td>' + ('Black' if game.current == game.black else 'White') + '</td></tr>\n'
            t1 = '<table border="1"> <tr><th>Black Score: </th><th>White Score: </th><th>Current Player: </th></tr>%s</table>' % (t1)
            cl.write(html_head)
            cl.write(t1)
            cl.write(t2_head)
            for i in range(8):
                cl.write(rows[i])
            cl.write(t2_foot)
            cl.write(html_foot)
            cl.close()

class Othello:
    empty = -1
    black = 0
    white = 1
    def __init__(self):
        self.size = 8
        self.board = []
        self.current = self.black
        for i in range(self.size):
            row = []
            for j in range(self.size):
                row.append(self.empty)
            self.board.append(row)
        startIndex = (self.size - 1) // 2
        self.board[startIndex][startIndex+1] = self.black
        self.board[startIndex][startIndex] = self.white
        self.board[startIndex+1][startIndex] = self.black
        self.board[startIndex+1][startIndex+1] = self.white
        self.promptTurn(self.current, self.size)
        self.numBlack = 2;
        self.numWhite = 2;

    def hasTurn(self, p):
        for i in range(self.size):
            for j in range(self.size):
                for k in range(8):
                    if self.board[i][j] == self.empty:
                        if self.checkCanFlip(p, i, j, k):
                            return True
        return False

    def flip(self, i, j):
        self.board[i][j] = (not self.board[i][j])

    def canFlip(self, p, i, j, d):
        iOffset = 0
        jOffset = 0
        if d == 0:
            iOffset = -1
            jOffset = -1
        elif d == 1:
            iOffset = -1
        elif d == 2:
            iOffset = -1
            jOffset = 1
        elif d == 3:
            jOffset = -1
        elif d == 4:
            jOffset = 1
        elif d == 5:
            iOffset = 1
            jOffset = -1
        elif d == 6:
            iOffset = 1
        elif d == 7:
            iOffset = 1
            jOffset = 1
        nextI = i + iOffset
        nextJ = j + jOffset

        if (nextI < 0 or nextI >= self.size or nextJ < 0 or nextJ >= self.size):
            return False
        elif (self.board[nextI][nextJ] == (not p) and self.board[i][j] == p):
            return self.canFlip(p, nextI, nextJ, d)
        elif (self.board[nextI][nextJ] == (not p) and self.board[i][j] == self.empty):
            nextCanFlip = self.canFlip(p, nextI, nextJ, d)
            if (nextCanFlip):
                self.board[i][j] = p
            return nextCanFlip
        elif (self.board[nextI][nextJ] == (not p) and self.board[i][j] == (not p)):
            nextCanFlip = self.canFlip(p, nextI, nextJ, d)
            if (nextCanFlip):
                self.flip(i, j)
            return nextCanFlip
        elif (self.board[nextI][nextJ] == p and self.board[i][j] == (not p)):
            self.flip(i, j)
            return True
        return False

    def checkCanFlip(self, p, i, j, d):
        iOffset = 0
        jOffset = 0
        if d == 0:
            iOffset = -1
            jOffset = -1
        elif d == 1:
            iOffset = -1
        elif d == 2:
            iOffset = -1
            jOffset = 1
        elif d == 3:
            jOffset = -1
        elif d == 4:
            jOffset = 1
        elif d == 5:
            iOffset = 1
            jOffset = -1
        elif d == 6:
            iOffset = 1
        elif d == 7:
            iOffset = 1
            jOffset = 1
        nextI = i + iOffset
        nextJ = j + jOffset

        if (nextI < 0 or nextI >= self.size or nextJ < 0 or nextJ >= self.size):
            return False
        if (self.board[nextI][nextJ] == self.empty):
            return False
        elif (self.board[nextI][nextJ] == p and self.board[i][j] == self.empty):
            return False
        elif (self.board[nextI][nextJ] == p and self.board[i][j] == (not p)):
            return True
        return self.checkCanFlip(p, nextI, nextJ, d)

    def checkTurn(self, p, i, j):
        valid = False
        if self.board[i][j] is not self.empty:
            return False
        for k in range(8):
            if self.canFlip(p, i, j, k):
                valid = True
        return valid

    def checkWin(self): 
        self.numBlack = 0
        self.numWhite = 0
        winner = -2
        for i in range(self.size):
            for j in range(self.size):
                if self.board[i][j] == self.empty:
                    winner = self.empty
                elif self.board[i][j] == self.black:
                    self.numBlack+=1
                elif self.board[i][j] == self.white:
                    self.numWhite+=1
        print("Player 1: " + str(self.numBlack) + "    Player 2: " + str(self.numWhite))
        if (self.numBlack == 0):
            winner = self.white
        elif (self.numWhite == 0):
            winner = self.black
        
        if (winner == self.empty):
            return winner
        elif (self.numBlack > self.numWhite):
            winner = self.black
        elif (self.numWhite > self.numBlack):
            winner = self.white
        print("Player " + str(winner + 1) + " won!")
        return winner

    def promptTurn(self, p, high):
        self.printBoard()
        print("Player " + str(p+1) + " turn, input 2 numbers (0 - " + str(high-1) + ") for row and col: ")
        print("call method makeTurn(i, j) on this object")

    def printBoard(self):
        print("  ", end='')
        for i in range(self.size):
            print(' '+str(i), end='')
        print()
        for i in range(self.size):
            print(' '+str(i), end='')
            for j in range(self.size):
                if (self.board[i][j] == self.black):
                    print(" ■", end='')
                elif (self.board[i][j] == self.white):
                    print(" □", end='')
                else:
                    print(" .", end='')
            print()

    def makeTurn(self, i, j):
        if (self.checkWin() == -1):
            if not self.checkTurn(self.current, i, j):
                print("Invalid input: Try Again")
                self.promptTurn(self.current, self.size)
                return
            if self.current == self.white:
                self.current = self.black
            else:
                self.current = self.white
            if not self.hasTurn(self.current):
                if self.current == self.white:
                    self.current = self.black
                else:
                    self.current = self.white
            self.promptTurn(self.current, self.size)

        else:
            self.printBoard()
        self.checkWin()

    def makeTurnWeb(self, i, j):
        if (self.checkWin() == -1):
            if not self.checkTurn(self.current, i, j):
                return -3
            if self.current == self.white:
                self.current = self.black
            else:
                self.current = self.white
            if not self.hasTurn(self.current):
                if self.current == self.white:
                    self.current = self.black
                else:
                    self.current = self.white
        return self.checkWin()


