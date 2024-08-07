from math import exp

def reluf(a):
    return max(0,a)
   
def sigmoidf(a):
    if a<=-700: return 0
    elif -700<a<700: return 1/(1+exp(-a))
    elif 700<=a: return 1

def linearv2(lst_layer,lst_weights):
	result=[]
	for weights in lst_weights:
		total=0
		for i,weight in enumerate(weights):
			r=weight*lst_layer[i]
			total+=r
		result.append(total)
	return result

def forward_pass(Network,X):
    if len(Network)==1:
        output=[]
        if "relu" in Network[0]:
            for i in X:
                output=output+[reluf(i)]
        if "sigmoid" in Network[0]:
            for i in range(len(X)):
                output=output+[sigmoidf(X[i])]
        if "linear" in Network[0][0]:
            output=output+linearv2(X,Network[0][1])
        return output
    elif len(Network)>1:
        lst=[]
        if "relu" in Network[0]:
            for i in range(len(X)):
                lst=lst+[reluf(X[i])]
        elif "sigmoid" in Network[0]:
            for i in range(len(X)):
                lst=lst+[sigmoidf(X[i])]
        elif "linear" in Network[0][0]:
            lst=lst+linearv2(X,Network[0][1])
        return forward_pass(Network[1:],lst)
