function [V,gradV]=spherHarmonicEval(C,S,point,a,c)
%#codegen

scalFactor=10^(-280);
fullyNormalized=true;
M = 12;

%If the coefficients are Schmidt-quasi-normalized, then convert them to
%fully normalized coefficients.
if(fullyNormalized==false)
    %Duplicate the input coefficients so that when they are modified, the
    %original values are not changed.
    for n=0:M
        %Deal with all of the other m values.
        k=1/sqrt(1+2*n);
        for m=0:n
            C(n+1,m+1)=k*C(n+1,m+1);
            S(n+1,m+1)=k*S(n+1,m+1);
        end
    end
end

%This stores all of the powers of a/r needed for the sum, regardless of
%which algorithm is used.
V=zeros(1,1);
gradV=zeros(3,1);

% for curPoint=1:numPoints
r=point(1);
lambda=point(2);
thetaCur=point(3);
    
nCoeff=zeros(M+1,1);
nCoeff(1)=1;
for n=1:M
    nCoeff(n+1)=nCoeff(n)*(a/r);
end

    if(abs(thetaCur)<88*pi/180||nargout<2)
        %At latitudes that are not near the poles, the algorithm of Holmes and
        %Featherstone is used. It can not be used for the gradient near the
        %poles, because of the singularity of the spherical coordinate system.
        
        %Compute the sine and cosine terms.
        [SinVec,CosVec]=calcSinCosTerms(lambda,M);
        
        theta=pi/2-thetaCur;
        u=sin(theta);      
        [PBarUVals,dPBarUValsdTheta]=NALegendreCosRat(theta,M,scalFactor);
        
        %Evaluate Equation 7 from the Holmes and Featherstone paper.
        XC=zeros(M+1,1);
        XS=zeros(M+1,1);
            
        for m=0:M
            for n=m:M
                XC(m+1)=XC(m+1)+nCoeff(n+1)*C(n+1,m+1)*PBarUVals(n+1,m+1);
                XS(m+1)=XS(m+1)+nCoeff(n+1)*S(n+1,m+1)*PBarUVals(n+1,m+1);
            end
        end
        
        %Use Horner's method to compute V.
        V=0;
        for m=M:-1:0
            OmegaRat=XC(m+1)*CosVec(m+1)+XS(m+1)*SinVec(m+1);
            V=V*u+OmegaRat;
        end
        
        %Multiply by the constant in front of the sum and get rid of the scale
        %factor.
        V=(c/r)*V/scalFactor;
        
        %If the gradient is desired.
        dVdr=0;
        dVdLambda=0;
        dVdTheta=0;
            
        XCdr=zeros(M+1,1);
        XSdr=zeros(M+1,1);
        XCdTheta=zeros(M+1,1);
        XSdTheta=zeros(M+1,1);
                
        %Evaluate Equation 7 from the Holmes and Featherstone paper.
        for m=0:M
            for n=m:M
                CScal=nCoeff(n+1)*C(n+1,m+1);
                SScal=nCoeff(n+1)*S(n+1,m+1);

                XCdr(m+1)=XCdr(m+1)+(n+1)*CScal*PBarUVals(n+1,m+1);
                XSdr(m+1)=XSdr(m+1)+(n+1)*SScal*PBarUVals(n+1,m+1);

                XCdTheta(m+1)=XCdTheta(m+1)+CScal*dPBarUValsdTheta(n+1,m+1);
                XSdTheta(m+1)=XSdTheta(m+1)+SScal*dPBarUValsdTheta(n+1,m+1);
            end
        end
  
        for m=M:-1:0
            OmegaRat=XCdr(m+1)*CosVec(m+1)+XSdr(m+1)*SinVec(m+1);
            dVdr=dVdr*u+OmegaRat;

            OmegaRat=m*(-XC(m+1)*SinVec(m+1)+XS(m+1)*CosVec(m+1));
            dVdLambda=dVdLambda*u+OmegaRat;

            OmegaRat=XCdTheta(m+1)*CosVec(m+1)+XSdTheta(m+1)*SinVec(m+1);
            dVdTheta=dVdTheta*u+OmegaRat;
        end
            
        dVdr=-(c/r^2)*dVdr/scalFactor;
        dVdLambda=(c/r)*dVdLambda/scalFactor;
        %The minus sign is because the input coordinate was with respect to
        %latitude, not the co-latitude that the NALegendreCosRat function uses.
        dVdTheta=-(c/r)*dVdTheta/scalFactor;

        gradV(:) = calcSpherJacob(point)'*[dVdr;dVdLambda;dVdTheta];
    end
end

function [SinVec,CosVec]=calcSinCosTerms(lambda,M)
    %Compute sin(m*lambda) and cos(m*lambda) for m=0 to m=M.
    SinVec=zeros(M+1,1);
    CosVec=zeros(M+1,1);
    %Explicitely set the first two terms.
    SinVec(0+1)=0;
    CosVec(0+1)=1;
    SinVec(1+1)=sin(lambda);
    CosVec(1+1)=cos(lambda);
    %Use a double angle identity to get the second order term.
    SinVec(2+1)=2*SinVec(1+1)*CosVec(1+1);
    CosVec(2+1)=1-2*SinVec(1+1)^2;
    %Use a two-part recursion for the rest of the terms.
    for m=3:M
        SinVec(m+1)=2*CosVec(1+1)*SinVec(m-1+1)-SinVec(m-2+1);
        CosVec(m+1)=2*CosVec(1+1)*CosVec(m-1+1)-CosVec(m-2+1);
    end
end
