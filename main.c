#include <msp430.h> 
#include <lcd.h>

int cuenta,cuenta_total,distancia_cm;
char u,d,c,aux;

int main(void)
{
 	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	

	P1REN|= BIT2;
	P1OUT|= ~BIT2;
	P1SEL|=BIT2;

	P2REN|=BIT1;
	P2OUT|=BIT1;

	P3DIR|=BIT0;
	P3REN|=0;

	TA0CTL=TASSEL__SMCLK + MC_2;
	TA0R|=0;

	TA0CCTL1|=CM_2 + CCIS_0 + CAP + SCS;
	TA0CCR1|=0;

	LCDinit();
    LCDclear();
    LCDline1(); LCDputStr("   Bienvenido");
    LCDline2(); LCDputStr("1:limpia 2:Mide");


	while(1)
	{

	    if((P2IN & BIT1)==0)
	    {
	        __delay_cycles(100);
	        if((P2IN & BIT1)==0)
	        {
	        P3OUT |= BIT0;
	        __delay_cycles(20);
	        P3OUT &= ~BIT0;
	        }
                  TA0R&=0;

                  LCDclear();

           while((P2IN&BIT1)==0)
               P3OUT&= ~BIT0;
       }

	    if((TA0CCTL1&CCIFG)==CCIFG)
	    {
	    cuenta=TA0CCR1;
	    cuenta_total=cuenta-473;
	    distancia_cm=((cuenta_total)*0.95367)/58;

        TA0CCTL1&=~CCIFG;
	    }

        u=distancia_cm%10;

        aux=distancia_cm-u;
        aux=aux%100;
        d=aux/10;

        aux=distancia_cm-d*10-u;
        c=aux/100;


        if(distancia_cm>99)
       {
          LCDline1(); LCDputChar(c+48); LCDputChar(d+48); LCDputChar(u+48); LCDputStr("  cm");
       }
        else
        if(distancia_cm>9)
        {
            LCDline1();  LCDputChar(d+48); LCDputChar(u+48); LCDputStr(" cm");
        }
        else
        if(distancia_cm>0)
        {
            LCDline1(); LCDputChar(u+48); LCDputStr(" cm");
        }

	}

}
