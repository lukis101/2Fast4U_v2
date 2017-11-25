
#ifndef TFFU_COMPASS_H_
#define TFFU_COMPASS_H_
#ifdef __cplusplus
 extern "C" {
#endif



class Compass
{
public:
	float direction;

	Compass();
	void Update();
};

#ifdef __cplusplus
}
#endif
#endif /* TFFU_COMPASS_H_ */
