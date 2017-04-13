import numpy as np
import matplotlib.pyplot as plt

def triangle(length, amplitude):
    section = length // 4
    x = np.linspace(0, amplitude, section+1)
    mx = -x
    return np.r_[x, x[-2::-1], mx[1:], mx[-2:0:-1]]

class RNN:
	def __init__(self, learningRate):
		self.Wxh = np.random.randn(15, 2)*0.01 # input to hidden
		self.mWxh = np.zeros_like(self.Wxh)
		self.Whh = np.random.randn(15, 15)*0.01 # hidden to hidden
		self.mWhh = np.zeros_like(self.Whh)
		self.Why = np.random.randn(2, 15)*0.01 # hidden to output
		self.mWhy = np.zeros_like(self.Why)
		self.Hprev = np.zeros((15,1))
		self.learningRate = learningRate

	def predict(self, X):
		#### FORWARD PASS ####
		Hnet = np.dot(self.Wxh, X) + np.dot(self.Whh, self.Hprev)  ### 15x1
		Hout = 1 / (1 + np.exp(-Hnet)) ### 15x1

		Ynet = np.dot(self.Why, Hout) ### 2x1
		Yout = 1 / (1 + np.exp(-Ynet)) ### 2x1
		return Yout

	def step(self, X, Target):
		#### FORWARD PASS ####
		Hnet = np.dot(self.Wxh, X) + np.dot(self.Whh, self.Hprev)  ### 15x1
		Hout = 1 / (1 + np.exp(-Hnet)) ### 15x1

		Ynet = np.dot(self.Why, Hout) ### 2x1
		Yout = 1 / (1 + np.exp(-Ynet)) ### 2x1

		E = ((Target - Yout)**2)/2

		#### BACKPROPAGATION ###########
		d_E_Yout = (Yout - Target) ### 2x1
		d_Yout_Ynet = (Yout)*(1-Yout) ### 2x1

		d_E_Ynet = d_E_Yout * d_Yout_Ynet ### 2x1

		d_Ynet_Why = Hout ### 15x1
		###################################################
		d_E_Why = np.dot(d_E_Ynet, d_Ynet_Why.T) ### 2x15
		###################################################

		d_Ynet_Hout = self.Why ### 2x15

		d_E_Hout = np.dot(d_Ynet_Hout.T, d_E_Ynet) ### 15x1

		d_Hout_dHnet = Hout*(1-Hout) ### 15x1

		d_E_Hnet = d_E_Hout * d_Hout_dHnet ### 15x1

		d_Hnet_Whh = self.Hprev ### 15x1
		###################################################
		d_E_Whh = np.dot(d_E_Hnet, d_Hnet_Whh.T) ### 15x15
		for m in xrange(15):
			d_E_Whh[m][m] = 0
		###################################################

		d_Hnet_Wxh = X ### 2x1
		############################################
		d_E_Wxh = np.dot(d_E_Hnet, d_Hnet_Wxh.T)
		############################################

		Hprev = Hout

		# perform parameter update with Adagrad
		for param, dparam, mem in zip([self.Wxh, self.Whh, self.Why],[d_E_Wxh, d_E_Whh, d_E_Why],[self.mWxh, self.mWhh, self.mWhy]):
			mem += dparam * dparam
    		param += -self.learningRate * dparam / np.sqrt(mem + 1e-8) # adagrad update

		return E, Yout


Fs = 640
f = 20
sample = 640
x = np.arange(sample)
y = np.sin(2 * np.pi * f * x / Fs)
xx = np.tile(list(triangle(64, 1)), 10)

network = RNN(learningRate=0.1)
Err = []

summ = 0

for i in xrange(10000):
	X = np.array([[xx[i%639]],[y[i%639]]])
	Target = np.array([[xx[i%639+1]],[y[i%639+1]]])
	e=network.step(X,Target)
	summ += e[0][0] + e[1][0]
	Err.append(summ/(i+1))

X = np.array([[xx[0]],[y[0]]])
XXX = [X[0][0]]
YYY = [X[1][0]]
for i in xrange(100):
	X = network.predict(X)
	XXX.append(X[0][0])
	YYY.append(X[1][0])

plt.plot(XXX,YYY)
#plt.plot(Err)
plt.show()
