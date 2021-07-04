int main(){
    int a,b,c[3],d;
    a=1*(2+3)%3;
    b= 1<5;
	println(a);
    println(b);
    c[0]=-20;
	c[1]=50;
	c[2]=-21;
    if(a && b)
	{
		 c[0]++;
	}
    else
        c[1]=c[0];
	a=c[0];
	b=c[1];
	d=c[2];
	
	 println(a);
	a=!c[0];
   
    println(b);
	println(d);
	println(a);
}
//2,1,-19,50,-21,0