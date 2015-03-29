import plotly.plotly as py
from plotly.graph_objs import *
py.sign_in("luchitatotoro", "bek4t5wvj0")

trace1 = Scatter(
    x=[10, 20, 30, 40, 50, 100, 250, 500, 750, 1000, 1250, 1500, 2000],
    y=[0.039, 0.092, 0.1561, 0.2288, 0.2872, 0.6276, 1.3876],
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