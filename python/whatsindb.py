import sqlite3

tweet_detail_db = '__HOME__/twitter/twitter.db'  # database to store mapping of user to emergency contact


def request_handler(request):
    if request['method'] == 'POST':
        display = request['form']['bool']
        handle = request['form']['user']
        conn = sqlite3.connect(tweet_detail_db)  # connect to that database (will create if it doesn't already exist)
        c = conn.cursor()  # move cursor into database (allows us to execute commands)
        who_to_dm = c.execute('''SELECT contact FROM emergency_table WHERE user = ?;''', (handle,)).fetchall()  # gets the emergency contact for this user
        conn.commit()  # commit commands
        conn.close()  # close connection to database
        return set(who_to_dm)