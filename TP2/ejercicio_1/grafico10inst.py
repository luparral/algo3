import plotly.plotly as py
from plotly.graph_objs import *
py.sign_in("luchitatotoro", "bek4t5wvj0")

trace1 = Scatter(
    x=[10, 50, 100, 250, 500, 750, 1000, 1250, 1500, 2000],
    y=[0.0128, 0.1281, 0.2337, 0.5953, 1.1326, 1.8624, 2.885, 3.0804, 3.7783, 5.0001],
	mode='lines+markers',
	line=Line(
    	shape='spline',
    	color='rgb(127, 255, 102)',
	)
	#marker=Marker(
	#	color='rgb(127, 255, 102)',
	#)
)	

#trace2 = Scatter(
#    x=[10, 50, 100, 200, 300, 400, 500, 1000, 1500, 2000, 2500, 3000],
#    y=[16, 5, 11, 9],
#	mode='lines+markers',
#	line=Line(
#    	shape='spline',
#    	color='rgb(255, 153, 102)',
#	)
	#marker=Marker(
	#	color='rgb(255, 153, 102)',
	#)
#)

#data = Data([trace1, trace2])
data = Data([trace1])
plot_url = py.plot(data, filename='basic-line')
