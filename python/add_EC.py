import sqlite3

tweet_detail_db = '__HOME__/twitter/twitter.db'  # database to store mapping of user to emergency contact


def request_handler(request):
    if request['method'] == 'POST':
        name = request['form']['handle']
        who_to_dm = request['form']['ec']
        conn = sqlite3.connect(tweet_detail_db)  # connect to that database (will create if it doesn't already exist)
        c = conn.cursor()  # move cursor into database (allows us to execute commands)
        c.execute('''CREATE TABLE IF NOT EXISTS emergency_table (user text, contact text);''')
        c.execute('''INSERT into emergency_table VALUES (?,?);''', (name, who_to_dm))
        conn.commit()  # commit commands
        conn.close()  # close connection to database
        return('database has added your emergency contact')