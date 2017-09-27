function [PBarUVals,dPBarUValsdTheta,d2PBarUValsdTheta2]=NALegendreCosRat(theta,M,scalFactor)
%#codegen
%NALEGENDRECOSRAT Evaluate \bar{P}_{nm}(cos(theta))/u^m for all n from
%                     0 to M and for each n for all m from 0 to n.
%                     Also evaluate D{\bar{P}_{nm}(cos(theta))}/u^m and 
%                     D2{\bar{P}_{nm}(cos(theta))}/u^m where D{} is the
%                     first derivative operator with respect to theta and
%                     D2{} is the second derivative operator with respect
%                     to theta. \bar{P}_{nm}(x) is the fully normalized
%                     associated Legendre function of x of degree n and
%                     order m. u=sin(theta). All of the values can be
%                     scaled by a factor of scalFac, if desired, to help
%                     prevent overflows with high degrees and orders.
%
%INPUTS: theta      An angle in radians.
%        maxDeg     The maximum degree and order of the output. This should
%                   be at least 2.
%     scalFactor    A scale factor to help prevent overflow of the results.
%                   In the Holmes and Featherstone paper, discussed below,
%                   a value of 10^(-280) is used.
%
    u=sin(theta);
    t=cos(theta);

    %Allocate space for the return variables. n ranges from 0 to M and m
    %ranges from 0 to n. Thus, the number of elements for each n ranges
    %from 1 to M+1. This means that a total of
    %(M+1)*(M+2)/2 elements are necessary.
    numPBarU=(M+1)*(M+2)/2;
    totalP=zeros(numPBarU,1);
    PBarUVals =zeros(length(totalP),length(totalP)); 
    
    %The value of PBar_{0,0}(cos(theta)) is independent of theta and is one. 
    PBarUVals(0+1,0+1)=1*scalFactor;

    %Set the seed value for PBar_{1,1}(cos(theta))/u from which the other
    %values will be computed.
    PBarUVals(1+1,1+1)=sqrt(3)*scalFactor;

    %Compute the values along the main diagonal, where m=n starting from
    %m=n=2. This implements equation 28 in the first Holmes and
    %Featherstone paper for the normalized associated Legendre function 
    %ratio.
    for m=2:M
        PBarUVals(m+1,m+1)=sqrt((2*m+1)/(2*m))*PBarUVals(m-1+1,m-1+1);
    end

    %Recursively compute the values using Equation 27 from the first Holmes
    %and Featherstone paper, taking into account the fact that the first
    %element of the recursion only has one term.
    jTerm=1/sqrt(2);
    %First, deal with the case where n=1, m=0;
    n=1;m=0;
    %g is given in Equation 18 of the first Holmes and Featherstone paper.
    g=2*(m+1)/sqrt((n-m)*(n+m+1));
    PBarUVals(n+1,m+1)=jTerm*g*t*PBarUVals(n+1,m+1+1);

    %Next, evaluate the values for all other valid n and m.
    for n=2:M
        %Deal with the first element of the recursion,which is  where m=n-1.
        m=n-1;
        g=2*(m+1)/sqrt((n-m)*(n+m+1));
        PBarUVals(n+1,m+1)=g*t*PBarUVals(n+1,m+1+1);
        
        %Recursively compute the values of the rest of the m terms.
        for m=(n-2):-1:1
            g=2*(m+1)/sqrt((n-m)*(n+m+1));
            %h is given in Equation 19 of the first Holmes and Featherstone
            %paper.
            h=sqrt((n+m+2)*(n-m-1)/((n-m)*(n+m+1)));
            PBarUVals(n+1,m+1)=g*t*PBarUVals(n+1,m+1+1)-h*u^2*PBarUVals(n+1,m+2+1);
        end

        %Deal with the special m=0 case.
        m=0;
        g=2*(m+1)/sqrt((n-m)*(n+m+1));
        h=sqrt((n+m+2)*(n-m-1)/((n-m)*(n+m+1)));
        PBarUVals(n+1,m+1)=jTerm*(g*t*PBarUVals(n+1,m+1+1)-h*u^2*PBarUVals(n+1,m+2+1));
    end
    
    %If the first derivative is desired.
    %Allocate space.
    dPBarUValsdTheta =zeros(length(totalP),length(totalP)); 
    %The first derivative of PBar_{0,0}(cos(theta)) is just zero.
    dPBarUValsdTheta(1,1)=0;
    m=1;

    %From Equation 30 in the first Holmes and Featherstone paper. This
    %is the seed value from which other values will be computed.
    dPBarUValsdTheta(1+1,1+1)=m*(t/u)*PBarUVals(1+1,1+1);
    
    %Compute the values along the main diagonal, where m=n starting 
    %from m=n=2. This implements Equation 30 in the frist Holmes and
    %Featherstone paper for the ratio of the first derivative. 
    for m=2:M
        dPBarUValsdTheta(m+1,m+1)=m*(t/u)*PBarUVals(m+1,m+1);
    end
        
    n=1;m=0;
    %e is given in Equation 22 of the first Holmes and Featherstone paper.
    e=sqrt((n+m+1)*(n-m)/2);
    %This is Equation 30 of the first Holmes and Featherstone paper for
    dPBarUValsdTheta(n+1,m+1)=-e*u*PBarUVals(n+1,m+1+1);
    
    %Next, evaluate the values for all other valid n and m.
    for n=2:M
        %Recursively compute the values of the m terms for m>0.
        for m=(n-1):-1:1
            e=sqrt((n+m+1)*(n-m));
            dPBarUValsdTheta(n+1,m+1)=m*(t/u)*PBarUVals(n+1,m+1)-e*u*PBarUVals(n+1,m+1+1);
        end
        %Deal with the special m=0 case.
        m=0;
        e=sqrt((n+m+1)*(n-m)/2);
        dPBarUValsdTheta(n+1,m+1)=-e*u*PBarUVals(n+1,m+1+1);
    end
    
    %If the second derivative is desired
    %Allocate space
    d2PBarUValsdTheta2 =zeros(length(totalP),length(totalP));
    for n=0:M
        for m=0:n
        %From the first (un-numbered) equation in the second Holmes and
        %Featherstone paper AFTER correction.
        d2PBarUValsdTheta2(n+1,m+1)=(m^2/u^2-n*(n+1))*PBarUVals(n+1,m+1)-(t/u)*dPBarUValsdTheta(n+1,m+1);
        end
    end
end

