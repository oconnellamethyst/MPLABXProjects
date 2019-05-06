#ifndef FLOWCONTROL_H
#define	FLOWCONTROL_H

#ifdef	__cplusplus
extern "C" {
#endif

    void flowControlInit(void);
    void waterTime(uint8_t water_time_in_seconds);
    //void pause(uint8_t pause_in_seconds);

#ifdef	__cplusplus
}
#endif

#endif	/* FLOWCONTROL_H */