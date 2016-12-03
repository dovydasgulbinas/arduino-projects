import SimpleHTTPServer
import SocketServer
import logging
import cgi
import serial
import sys
import json
import signal

"""
Known issues:
1. On the first round of the game no game start data can be received(running at freezetime, freeztime over)
"""

PORT = 3000
first_call = True

# If we ctrl+C out of python, make sure we close the serial port first
# handler catches and closes it
def signal_handler(signal, frame):
    ser.close()
    sys.exit(0)

class ServerHandler(SimpleHTTPServer.SimpleHTTPRequestHandler):
    score_ct = 0
    score_t = 0
    # stores the number of kills player got in one round
    round_kills = 0

    def disp_bomb_planted(self, serial_cmd=0, countdown_duration=40):  # countdown until freezetime ends
        serial_string = ("%d,%d;") % (serial_cmd, countdown_duration)
        ser.write(serial_string)

    def disp_start_countdown(self, serial_cmd=1, countdown_duration=15):  # countdown until freezetime ends
        serial_string = ("%d,%d;") % (serial_cmd, countdown_duration)
        ser.write(serial_string)

    def disp_stop_countdown(self, serial_cmd=2):
        serial_string = ("%d;") % (serial_cmd)
        ser.write(serial_string)

    def disp_got_kill(self, serial_cmd=3):
        serial_string = ("%d;") % (serial_cmd)
        ser.write(serial_string)

    def disp_score(self, serial_cmd=4, score_ct=0, score_t=0):
        serial_string = ("%d,%d,%d;") % (serial_cmd, score_ct, score_t)
        ser.write(serial_string)

    def disp_init(self, serial_cmd=5, run=1):  # turn on the display and send let him know that game has started
        serial_string = ("%d,%d;") % (serial_cmd, run)  # 1 inits the game display, 0 disables game display
        ser.write(serial_string)

    def update_team_scores(self, jsonDict):
        # will parse team scores to global variables so that other methods can access this data more easily
        if 'map' in jsonDict:  # get ct and t team scores
            scores = jsonDict['map']  # get map object/dict
            self.score_ct = int(scores['team_ct']["score"])  # map >> team_ct >> score
            self.score_t = int(scores['team_t']["score"])

    def get_condition_object(self, jsonDict, parent_object, child_object, childs_condition):
        if parent_object in jsonDict:
            temp = jsonDict[parent_object]  # checks whether child object can be found in the parent string
            if child_object in temp:  # error occurs in this live when leaving the game investigate json
                if temp[child_object] == childs_condition:
                    return True
                else:
                    return False
            else:
                return False
        else:
            return False

    def get_from_super_object(self, jsonDict, super_parent_object, parent_object, child_object, childs_condition):
        if super_parent_object in jsonDict:
            return self.get_condition_object(jsonDict[super_parent_object], parent_object, child_object,
                                             childs_condition)
        else:
            return False

    def get_int_object(self, jsonDict, super_parent_object, parent_object, child_object):
        if super_parent_object in jsonDict:
            super_parent = jsonDict[super_parent_object]
            if parent_object in super_parent:
                parent = super_parent[parent_object]
                if child_object in parent:
                    return int(parent[child_object])
                else:
                    return -1
            else:
                return -1
        else:
            return -1

    # this is a bomb plant trigger it is fired once per round, remember bomb can also be planted after time runs out
    def bomb_planted(self, jsonDict):
        return self.get_from_super_object(jsonDict, 'added', 'round', 'bomb', True)

    def freezetime_started(self, jsonDict):  # will return true if freeztime occured
        return self.get_from_super_object(jsonDict, 'previously', 'round', 'phase', 'over') and \
               self.get_condition_object(jsonDict, 'round', 'phase', 'freezetime')

    # Every time round ends this will return true:
    # (round time ran out, bomb defused, bomb exploded, all ct killed, all t killed)
    def round_over(self, jsonDict):
        return self.get_from_super_object(jsonDict, 'previously', 'round', 'phase', 'live') and \
               self.get_condition_object(jsonDict, 'round', 'phase', 'over')

    # *******************************************************************
    def bomb_defused(self, jsonDict):
        return self.get_condition_object(jsonDict, 'round', 'bomb', 'defused')

    def bomb_exploded(self, jsonDict):
        return self.get_condition_object(jsonDict, 'round', 'bomb', 'exploded')

    def ct_win(self, jsonDict):
        return self.get_condition_object(jsonDict, 'round', 'win_team', 'CT')

    def t_win(self, jsonDict):
        return self.get_condition_object(jsonDict, 'round', 'win_team', 'T')

    # *******************************************************************

    def player_got_kill(self, jsonDict):
        if 'previously' in jsonDict:
            prev_round_kills = self.get_int_object(jsonDict['previously'], 'player', 'state', 'round_kills')
            if prev_round_kills == -1:
                return False
            # get number of frags received current round by the player spectated
            self.round_kills = self.get_int_object(jsonDict, 'player', 'state', 'round_kills')
            if self.round_kills - prev_round_kills > 0:
                return True
            else:
                return False
        else:
            return False

    # freezetime is over meaning that new round time began
    def freezetime_over(self, jsonDict):
        return self.get_from_super_object(jsonDict, 'previously', 'round', 'phase', 'freezetime') and \
               self.get_condition_object(jsonDict, 'round', 'phase', 'live')

    def on_first_run(self, jsonDict):
        global first_call
        if first_call:
            self.disp_init()
            if (self.get_condition_object(jsonDict, 'round', 'phase', 'freezetime')):  # @freeze-time
                self.disp_start_countdown(countdown_duration=15)
                first_call = False
            if (self.get_condition_object(jsonDict, 'round', 'phase', 'live')):  # freeze-time is over or heartbeat is sent
                self.disp_start_countdown(countdown_duration=115)
                first_call = False
            print "First json with competitive game data"


    def do_GET(self):
        SimpleHTTPServer.SimpleHTTPRequestHandler.do_GET(self)

    # SimpleHTTPServer doesn't handle post by default. Hacked up way to do it here
    def do_POST(self):
        length = int(self.headers["Content-Length"])
        jsonString = str(self.rfile.read(length))
        # print jsonString
        print "---"
        jsonDict = json.loads(jsonString)

       # self.on_first_run(jsonString)

        self.update_team_scores(jsonDict)

        if self.freezetime_started(jsonDict):  # freezetime event occured lets do something
            self.disp_start_countdown()
            print " AT FREEZE TIME - CT:%d | T:%d" % (self.score_ct, self.score_t)


        # This will notify you when freezetime is over
        if self.freezetime_over(jsonDict):
            self.disp_start_countdown(countdown_duration=115)
            print "PICK UP YOUR WEAPONS AND FIGHT!"


        if self.player_got_kill(jsonDict):
            self.disp_got_kill()
            print 'YOU GOT A KILL, # of enemies killed this round: ', self.round_kills

        if self.bomb_planted(jsonDict):
            self.disp_bomb_planted()
            print "BOMB HAS BEEN PLANTED!"



        # round over event will occur on the following cases:
        # (round time ran out, bomb defused, bomb exploded, all ct killed, all t killed)
        # all of the events below are nested in order to get greater efficiency in code you can call all of the methods independently as well
        if self.round_over(jsonDict):
            if self.bomb_defused(jsonDict):
                self.disp_score(score_ct=self.score_ct + 1, score_t=self.score_t)
                print "CT DEFUSED THE BOMB"
            elif self.bomb_exploded(jsonDict):
                self.disp_score(score_ct=self.score_ct, score_t=self.score_t + 1)
                print "T DETONATED THE BOMB"
            elif self.ct_win(jsonDict):
                self.disp_score(score_ct=self.score_ct + 1, score_t=self.score_t)
                print "CT SHOT ALL THE T OR TIME RAN OUT"
            elif self.t_win(jsonDict):  # elif is used for some efficiency
                self.disp_score(score_ct=self.score_ct, score_t=self.score_t + 1)
                print "T SHOT ALL THE CT"

        # Sends a response to your game, this is required
        response = bytes("This is the response.")  # create response
        self.send_response(200)  # create header
        self.send_header("Content-Length", str(len(response)))
        self.end_headers()
        self.wfile.write(response)  # send response


Handler = ServerHandler
ser = serial.Serial('COM3')
# Set up our handler for ctrl+c
signal.signal(signal.SIGINT, signal_handler)

# Start server
httpd = SocketServer.TCPServer(("", PORT), Handler)

# Run server
httpd.serve_forever()
