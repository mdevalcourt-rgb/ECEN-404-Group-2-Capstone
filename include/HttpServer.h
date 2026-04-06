#pragma once

#include <WebServer.h>

class Ad7356Sampler;
class Ad9833Driver;
class Ads1115Sampler;
class Mic24045;

WebServer &httpServer();
void setupServerRoutes(Ad7356Sampler &sampler, Ads1115Sampler &ads,
                       Mic24045 &mic, Ad9833Driver &wave);
