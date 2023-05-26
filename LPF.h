class  BW_Filter
{
	public:
		BW_Filter()
		{
			v[0]=v[1]=0.0;
		}
	private:
		float v[2]; // size
	public:
		float step(float x) //class II 
		{
			v[0] = v[1]; // first and 2nd ele
			v[1] = (9.845337085968967061e-1 * x) + (-0.96906741719379341227 * v[0]);
			
      return (v[0] + v[1]);
		}
};

