int main(){
    int a,b,c[3];
	a=1*(2+3)%3;
    b= 1<5;
    c[0]=20;
	c[1]=10;
	println(a);
    println(b);
    if(a && b)
        c[0]++;
    else
        c[1]=c[0];

	a=c[0];
	b=c[1];
    println(a);
    println(b);
}//2,1,21,10
