from visual import*
from visual.graph import*
#mass of floor
m=0.05#mass of balls
m2=0.02
M=0.03
g=9.8
L=0.05
N=2
tcheck=0.0
tcheckrun=0.0
checkt=0.0
theta=pi**0/360
flag=0
flllag=0
point=0

findperiod=0

scene=display(width=800,height=500,center=(0.05*0,0,0.05*N),background=(0.5,0.5,0))
scene2=gdisplay(x=0,y=500,width=800,height=300,ytitle='delta_z',xtitle='t',background=(0.4,0.4,0))
p=gcurve(color=color.blue,gdisplay=scene2)
p2=gcurve(color=color.red,gdisplay=scene2)
p3=gcurve(color=color.cyan,gdisplay=scene2)
p_3=gcurve(color=color.white,gdisplay=scene2)
p4=gdots(color=color.cyan,gdisplay=scene2)
floor=box(length=1*(10+1)/100.0,height=0.001,width=N*(10+1)/100.0,color=color.blue)
disks=[cylinder(pos=vector(0.1*0,0,0.1*i), axis = (0, 0.1, 0), radius=0.0025, material = materials.wood)for i in range(N) ]
disks2=[cylinder(pos=vector(0.1*0,0.1,0.1*i), axis = (0.025, 0, 0), radius=0.0025, material = materials.wood)for i in range(N) ]
ropes=[cylinder(pos=vector(0.1*0+0.0125,0.1,0.1*i), axis = (0, -0.5, 0), radius=0.001, material = materials.wood)for i in range(N) ]
ropes2=[cylinder(pos=vector(0.1*0+0.0125,0.1,0.1*i), axis = (0, 0.5, 0), radius=0.001, material = materials.wood)for i in range(N) ]
balls=[sphere(pos=vector(0.1*0+0.0125,0.1-L+L*(1.0-cos(theta)),0.1*i+L*sin(theta)),radius=0.02,material = materials.wood)for i in range(N)]
balls2=[sphere(pos=vector((0.1*0+0.0125),0.1-(0.1-L+L*(1.0-cos(theta))),0.2*i-(0.1*i+L*sin(theta))),radius=0.01,material = materials.wood)for i in range(N) ]
if (N%2==1):
    floor.pos=disks[N/2].pos
if (N%2==0):
    floor.pos=(disks[(N)/2-1].pos+disks[(N)/2].pos)/2
floor.a=vector(0.0,0.0,0.0)
floor.v=vector(0.0,0.0,0.0)
dt=0.0001
t=0
t2=0
def CMX():
    pos=M*floor.pos
    for i in range(N):
        pos+=m*balls[i].pos+m2*balls2[i].pos
    pos=pos/(M+N*(m+m2))
    pos.y=0.05
    return pos
def F_air_drag(V):
    Vmag=mag(V)
    F=(1*0.4*Vmag**1.5)*(-V.norm())
    #F=vector(0,0,0)
    return F
def ball_air_drag(V):
    f=(2*0.4*abs(V)**2)
    return f
def settheta(i,theta):
    balls[i].theta=theta
    balls2[i].theta=theta
    balls[i].ome=0.0
    balls2[i].ome=0.0
    balls[i].ome=0.0
    balls2[i].ome=0.0
    ropes[i].axis=balls[i].pos-ropes[i].pos
    ropes2[i].axis=balls2[i].pos-ropes2[i].pos
    balls[i].pos=vector(0,-L*cos(balls[i].theta), 0+L*sin(balls[i].theta))+ropes[i].pos
    balls2[i].pos=vector(0,L*cos(balls[i].theta), 0-L*sin(balls[i].theta))+ropes[i].pos
def reset(self):
    t=0
    tcheckrun=0
    floor.v=vector(0,0,0)
    floor.a=vector(0,0,0)
    for i in range(N):
        disks[i].pos=vector(0.1*0,0,0.1*i)
        disks2[i].pos=vector(0.1*0,0.1,0.1*i)
        ropes[i].pos=vector(0.1*0+0.0125,0.1,0.1*i)
        ropes2[i].pos=vector(0.1*0+0.0125,0.1,0.1*i)
        balls[i].pos=vector(0.1*0+0.0125,0.1-L+L*(1.0-cos(theta)),0.1*i+L*sin(theta))
        balls2[i].pos=vector((0.1*0+0.0125),0.1-(0.1-L+L*(1.0-cos(theta))),0.2*i-(0.1*i+L*sin(theta)))
    if (N%2==1):
        floor.pos=disks[N/2].pos
    if (N%2==0):
        floor.pos=(disks[(N)/2-1].pos+disks[(N)/2].pos)/2
    for i in range(N):
        balls[i].a=vector(0.0,0.0,0.0)
        balls[i].alpha=0.0
        balls[i].ome=0.0
        balls[i].theta=0.25
        balls[i].v=vector(0.0,0.0,0.0)
        balls2[i].a=vector(0.0,0.0,0.0)
        balls2[i].v=vector(0.0,0.0,0.0)
        balls2[i].alpha=0.0
        balls2[i].ome=0.0
        balls2[i].theta=0.25
        balls[i].pos=vector(0.0125,0.1-L*cos(balls[i].theta), 0.1*i+L*sin(balls[i].theta))+floor.v*dt
        balls2[i].pos=vector(0.0125,0.1+L*cos(balls[i].theta), 0.1*i-L*sin(balls[i].theta))+floor.v*dt
    for i in range(N):
        #balls[i].a=(((vector(0,-g,0)*m+(-k*(abs(balls[i].pos-ropes[i].pos)-L)*ropes[i].axis.norm())).cross(ropes[i].axis)*-1+(vector(0,-g,0)*m2+(-k*(abs(balls2[i].pos-ropes2[i].pos)-L)*ropes2[i].axis.norm())).cross(ropes2[i].axis)*-1)/(m*mag2(ropes[i].axis)+m2*mag2(ropes2[i].axis))).cross(ropes[i].axis)
        #balls[i].v+=balls[i].a*dt
        #balls[i].pos+=balls[i].v*dt
        #balls2[i].a=(((vector(0,-g,0)*m+(-k*(abs(balls[i].pos-ropes[i].pos)-L)*ropes[i].axis.norm())).cross(ropes[i].axis)*-1+(vector(0,-g,0)*m2+(-k*(abs(balls2[i].pos-ropes2[i].pos)-L)*ropes2[i].axis.norm())).cross(ropes2[i].axis)*-1)/((m*mag2(ropes[i].axis)+m2*mag2(ropes2[i].axis)))).cross(ropes2[i].axis)#vector(0,-g,0)+(-k*(abs(balls2[i].pos-ropes2[i].pos)-L)*ropes2[i].axis.norm())/m
        #balls2[i].v+=balls2[i].a*dt
        #balls2[i].pos+=balls2[i].v*dt
        ropes[i].axis=balls[i].pos-ropes[i].pos
        ropes2[i].axis=balls2[i].pos-ropes2[i].pos
    for i in range(N):
        balls[i].a=((ropes[i].axis.cross(m*vector(0,-g,0))+ropes2[i].axis.cross(m2*vector(0,-g,0))+ropes[i].axis.cross(-floor.a*m)+ropes2[i].axis.cross(-floor.a*m2))/(m*mag2(ropes[i].axis)+m2*mag2(ropes2[i].axis))).cross(ropes[i].axis)
        balls[i].a-=ball_air_drag(balls[i].v)*balls[i].a.norm()#有改
        balls2[i].a=((ropes[i].axis.cross(m*vector(0,-g,0))+ropes2[i].axis.cross(m2*vector(0,-g,0))+ropes[i].axis.cross(-floor.a*m)+ropes2[i].axis.cross(-floor.a*m2))/(m*mag2(ropes[i].axis)+m2*mag2(ropes2[i].axis))).cross(ropes2[i].axis)
        balls2[i].a-=ball_air_drag(balls2[i].v)*balls2[i].a.norm()#有改
    for i in range(N):
        balls[i].alpha=(-((ropes[i].axis.cross(m*vector(0,-g,0))+ropes2[i].axis.cross(m2*vector(0,-g,0))+ropes[i].axis.cross(-floor.a*m)+ropes2[i].axis.cross(-floor.a*m2))/(m*mag2(ropes[i].axis)+m2*mag2(ropes2[i].axis))).x)+ball_air_drag(balls[i].v)/abs(ropes[i].axis)+ball_air_drag(balls2[i].v)/abs(ropes2[i].axis)#有改
        balls2[i].alpha=(-((ropes[i].axis.cross(m*vector(0,-g,0))+ropes2[i].axis.cross(m2*vector(0,-g,0))+ropes[i].axis.cross(-floor.a*m)+ropes2[i].axis.cross(-floor.a*m2))/(m*mag2(ropes[i].axis)+m2*mag2(ropes2[i].axis))).x)+ball_air_drag(balls[i].v)/abs(ropes[i].axis)+ball_air_drag(balls2[i].v)/abs(ropes2[i].axis)#有改
    #scene.center=floor.pos+vector(0,0.05,0)     
def changedata():
    global M
    M+=0.005
def printdata():
    print "M = ",M,",t = ",t
for i in range(N):
    balls[i].a=vector(0.0,0.0,0.0)
    balls[i].alpha=0.0
    balls[i].ome=0.0
    balls[i].theta=0.25
    balls[i].v=vector(0.0,0.0,0.0)
    balls2[i].a=vector(0.0,0.0,0.0)
    balls2[i].v=vector(0.0,0.0,0.0)
    balls2[i].alpha=0.0
    balls2[i].ome=0.0
    balls2[i].theta=0.25
    balls[i].pos=vector(0.0125,0.1-L*cos(balls[i].theta), 0.1*i+L*sin(balls[i].theta))+floor.v*dt
    balls2[i].pos=vector(0.0125,0.1+L*cos(balls[i].theta), 0.1*i-L*sin(balls[i].theta))+floor.v*dt
for i in range(N):
    #balls[i].a=(((vector(0,-g,0)*m+(-k*(abs(balls[i].pos-ropes[i].pos)-L)*ropes[i].axis.norm())).cross(ropes[i].axis)*-1+(vector(0,-g,0)*m2+(-k*(abs(balls2[i].pos-ropes2[i].pos)-L)*ropes2[i].axis.norm())).cross(ropes2[i].axis)*-1)/(m*mag2(ropes[i].axis)+m2*mag2(ropes2[i].axis))).cross(ropes[i].axis)
    #balls[i].v+=balls[i].a*dt
    #balls[i].pos+=balls[i].v*dt
    #balls2[i].a=(((vector(0,-g,0)*m+(-k*(abs(balls[i].pos-ropes[i].pos)-L)*ropes[i].axis.norm())).cross(ropes[i].axis)*-1+(vector(0,-g,0)*m2+(-k*(abs(balls2[i].pos-ropes2[i].pos)-L)*ropes2[i].axis.norm())).cross(ropes2[i].axis)*-1)/((m*mag2(ropes[i].axis)+m2*mag2(ropes2[i].axis)))).cross(ropes2[i].axis)#vector(0,-g,0)+(-k*(abs(balls2[i].pos-ropes2[i].pos)-L)*ropes2[i].axis.norm())/m
    #balls2[i].v+=balls2[i].a*dt
    #balls2[i].pos+=balls2[i].v*dt
    ropes[i].axis=balls[i].pos-ropes[i].pos
    ropes2[i].axis=balls2[i].pos-ropes2[i].pos

for i in range(N):
    balls[i].a=((ropes[i].axis.cross(m*vector(0,-g,0))+ropes2[i].axis.cross(m2*vector(0,-g,0))+ropes[i].axis.cross(-floor.a*m)+ropes2[i].axis.cross(-floor.a*m2))/(m*mag2(ropes[i].axis)+m2*mag2(ropes2[i].axis))).cross(ropes[i].axis)
    balls[i].a-=ball_air_drag(balls[i].v)*balls[i].a.norm()#有改
    balls2[i].a=((ropes[i].axis.cross(m*vector(0,-g,0))+ropes2[i].axis.cross(m2*vector(0,-g,0))+ropes[i].axis.cross(-floor.a*m)+ropes2[i].axis.cross(-floor.a*m2))/(m*mag2(ropes[i].axis)+m2*mag2(ropes2[i].axis))).cross(ropes2[i].axis)
    balls2[i].a-=ball_air_drag(balls2[i].v)*balls2[i].a.norm()#有改
for i in range(N):
    balls[i].alpha=(-((ropes[i].axis.cross(m*vector(0,-g,0))+ropes2[i].axis.cross(m2*vector(0,-g,0))+ropes[i].axis.cross(-floor.a*m)+ropes2[i].axis.cross(-floor.a*m2))/(m*mag2(ropes[i].axis)+m2*mag2(ropes2[i].axis))).x)+ball_air_drag(balls[i].v)/abs(ropes[i].axis)+ball_air_drag(balls2[i].v)/abs(ropes2[i].axis)#有改
    balls2[i].alpha=(-((ropes[i].axis.cross(m*vector(0,-g,0))+ropes2[i].axis.cross(m2*vector(0,-g,0))+ropes[i].axis.cross(-floor.a*m)+ropes2[i].axis.cross(-floor.a*m2))/(m*mag2(ropes[i].axis)+m2*mag2(ropes2[i].axis))).x)+ball_air_drag(balls[i].v)/abs(ropes[i].axis)+ball_air_drag(balls2[i].v)/abs(ropes2[i].axis)#有改
#scene.center=floor.pos+vector(0,0.05,0)     

scene.forward=vector(-1,0,0)
while True:
    rate(2000)
    scene.center=CMX()
    t+=dt
    t2+=1
    tcheckrun+=dt

    if(t>=0 and t<3000*dt):
        settheta(0,-0.25)
    #if(t>=0 and t<2000*dt):
     #   settheta(1,-0.25)
    #if(point==5):
     #   printdata()
      #  changedata()
       # reset(1)
       # point=0
        #t=0
    bpz=balls[0].pos.z-ropes[0].pos.z
    for i in range(N):
        balls[i].a=((ropes[i].axis.cross(m*vector(0,-g,0))+ropes2[i].axis.cross(m2*vector(0,-g,0))+ropes[i].axis.cross(-floor.a*m)+ropes2[i].axis.cross(-floor.a*m2))/(m*mag2(ropes[i].axis)+m2*mag2(ropes2[i].axis))).cross(ropes[i].axis)
        balls[i].a-=ball_air_drag(balls[i].v)*balls[i].a.norm()#有改
        balls2[i].a=((ropes[i].axis.cross(m*vector(0,-g,0))+ropes2[i].axis.cross(m2*vector(0,-g,0))+ropes[i].axis.cross(-floor.a*m)+ropes2[i].axis.cross(-floor.a*m2))/(m*mag2(ropes[i].axis)+m2*mag2(ropes2[i].axis))).cross(ropes2[i].axis)
        balls2[i].a-=ball_air_drag(balls2[i].v)*balls2[i].a.norm()#有改
    if(t>=3000*dt):
        floor.a=vector(0,0,0)
        for i in range(N):
            floor.a+=-(balls[i].a*m+balls2[i].a*m2)/M+F_air_drag(floor.v)/M
        floor.a.y=0
        
    for i in range(N):
        balls[i].alpha=(-((ropes[i].axis.cross(m*vector(0,-g,0))+ropes2[i].axis.cross(m2*vector(0,-g,0))+ropes[i].axis.cross(-floor.a*m)+ropes2[i].axis.cross(-floor.a*m2)+ropes[i].axis.cross(ball_air_drag(balls[i].v)*(-balls[i].a.norm()))+ropes2[i].axis.cross(ball_air_drag(balls2[i].v)*(-balls2[i].a.norm())))/(m*mag2(ropes[i].axis)+m2*mag2(ropes2[i].axis))).x)
        balls2[i].alpha=(-((ropes[i].axis.cross(m*vector(0,-g,0))+ropes2[i].axis.cross(m2*vector(0,-g,0))+ropes[i].axis.cross(-floor.a*m)+ropes2[i].axis.cross(-floor.a*m2)+ropes[i].axis.cross(ball_air_drag(balls[i].v)*(-balls[i].a.norm()))+ropes2[i].axis.cross(ball_air_drag(balls2[i].v)*(-balls2[i].a.norm())))/(m*mag2(ropes[i].axis)+m2*mag2(ropes2[i].axis))).x)
    for i in range(N):
        balls[i].ome+=balls[i].alpha*dt
        balls2[i].ome+=balls2[i].alpha*dt
        balls[i].v=balls[i].ome*abs(ropes[i].axis)#有改
        balls2[i].v=balls2[i].ome*abs(ropes[i].axis)#有改
        ropes[i].v=floor.v
        ropes2[i].v=floor.v
    floor.v+=floor.a*dt
    for i in range(N):
        balls[i].theta+=balls[i].ome*dt
        balls2[i].theta+=balls2[i].ome*dt
        ropes[i].axis=balls[i].pos-ropes[i].pos
        ropes2[i].axis=balls2[i].pos-ropes2[i].pos
        ropes[i].pos+=ropes[i].v*dt
        ropes2[i].pos+=ropes2[i].v*dt
        balls[i].pos=vector(0,-L*cos(balls[i].theta), 0+L*sin(balls[i].theta))+ropes[i].pos
        balls2[i].pos=vector(0,L*cos(balls[i].theta), 0-L*sin(balls[i].theta))+ropes[i].pos
        
    
        disks[i].pos+=floor.v*dt
        disks2[i].pos+=floor.v*dt
    
    floor.pos+=floor.v*dt
    if(t<=50):
        p.plot(pos=(t,balls[0].pos.z-ropes[0].pos.z))
        p2.plot(pos=(t,balls[1].pos.z-ropes[1].pos.z))
        #p_3.plot(pos=(t,balls[2].pos.z-ropes[2].pos.z))
        p3.plot(pos=(t,balls[0].pos.z-ropes[0].pos.z+balls[1].pos.z-ropes[1].pos.z))#+balls[2].pos.z-ropes[2].pos.z
        if(flllag==0 and abs(balls[0].pos.z-ropes[0].pos.z)+abs(balls[1].pos.z-ropes[1].pos.z)<=0.001):#+abs(balls[2].pos.z-ropes[2].pos.z)
            p4.plot(pos=(t,0))
            point+=1
            flllag=1
            print point
        if(abs(balls[0].pos.z-ropes[0].pos.z)+abs(balls[1].pos.z-ropes[1].pos.z)>0.001):
            flllag=0
    if(balls[0].pos.z-ropes[0].pos.z>bpz):
        if(flag==0):
            tcheckrun=0
            flag=1
        if(flag==2):
            if(t>checkt+(20+0.5)*tcheck):
                tcheck=tcheckrun
                checkt=t
                point=0
                findperiod=1
                print " ",tcheckrun
                tcheckrun=0
            flag=0
    if(balls[0].pos.z-ropes[0].pos.z<bpz):
        if(flag==1):
            flag=2
    if(t>checkt+(20+0.5)*tcheck):
        findperiod=0
    if(findperiod==0):
        point=0
    
    #if(t>=1):
        #print "POS of CM = ",CM()
        #print "Energy = ",(m*mag2(balls[0].v)+m*mag2(balls[1].v)+m*mag2(balls[2].v)+m*mag2(balls[3].v)+M*mag2(floor.v))*0.5+m*g*(balls[0].pos.y+balls[1].pos.y+balls[2].pos.y+balls[3].pos.y)
        #E=(m*mag2(balls[0].v)+m*mag2(balls[1].v)+m*mag2(balls[2].v)+m*mag2(balls[3].v)+M*mag2(floor.v))*0.5+m*g*(balls[0].pos.y+balls[1].pos.y+balls[2].pos.y+balls[3].pos.y)
        #print "energy of balls[0]-averge energy = ",0.5*m*mag2(balls[0].v)+m*g*balls[0].pos.y-E/4
        #t=0
