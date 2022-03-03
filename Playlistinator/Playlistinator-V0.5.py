## Playlistinator V0.5
## By William Hollywood

import sys
import os

error = ''
try:
	import ffmpeg
except:
	error += '	' + str(sys.exc_info()[1]) + '\n'
try:
	import ffprobe
except:
	error += '	' + str(sys.exc_info()[1]) + '\n'
try:
	import spotipy
	import spotipy.util as util
except:
	error += '	' + str(sys.exc_info()[1]) + '\n'
try:
	import youtube_dl
except:
	error += '	' + str(sys.exc_info()[1]) + '\n'
try:
	import unidecode
except:
	error += '	' + str(sys.exc_info()[1]) + '\n'

	
if error != '':
	print 'Missing library(s) required: \n', error
	gotAns = False
	while gotAns == False:
		ans = raw_input('Would you like to download it/them? (Y/N)\n')
		if ans == 'y' or ans == 'Y':
			gotAns = True
			os.system('pip install -r requirements.txt')
		elif ans == 'n' or ans == 'N':
			print 'To install requirements manually please do: \'pip install -r requirements.txt\''
			os.exit()
		else:	
			print 'Answer must be \'Y\', \'y\', \'N\' or \'n\''

import ffmpeg
import ffprobe
import spotipy
import spotipy.util as util
import youtube_dl
import unidecode

ydl_opts = {
    'format': 'bestaudio/best',
    'postprocessors': [{
        'key': 'FFmpegExtractAudio',
        'preferredcodec': 'mp3',
        'preferredquality': '256',
    }],
}

scope = 'user-library-read'

if len(sys.argv) > 2:
    username = sys.argv[1]
    playlistID = sys.argv[2]
else:
    print "Usage: %s username playlist_id" % (sys.argv[0],)
    ##sys.exit()

##username = "PUT EMAIL HERE"
##playlistID = 'ID OF PLAYLIST TO DOWNLOAD'

token = util.prompt_for_user_token(username,scope,client_id='TOKEN',client_secret='TOKEN',redirect_uri='TOKEN')

if token:
	sp = spotipy.Spotify(auth=token)
	playlist_tracks = sp.user_playlist_tracks(username, playlistID, fields='items,uri,name,id,total',limit=100, market=None)
	
	for item in playlist_tracks['items']:
		track = item['track']
		print track['name'] + ' - ' + track['artists'][0]['name']
		with youtube_dl.YoutubeDL(ydl_opts) as ydl:
			##filenames = sys.argv[1:]
			query = os.system(unidecode.unidecode('youtube-dl --extract-audio --audio-format mp3 -o "./PlaylistinatorOut/%(title)s.%(ext)s" "ytsearch1:'+track['name'] + ' - ' + track['artists'][0]['name']+'"'))
			print query
			#ydl.download(query)
else:
    print "Can't get token for", username
