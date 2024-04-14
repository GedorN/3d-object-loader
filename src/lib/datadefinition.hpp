#pragma once

enum VisualizationMode {
  FILLED,
  WIREFRAME
};

enum ManipulationMode {
  TRANSLATING,
  ROTATING,
  SCALING
};

enum EntityManipuled {
  MESH,
  OBSERVER
};

typedef struct {
  float x = .0f;
  float y = .0f;
  float z = .0f;
} PlanCoords3d;

typedef struct {
  float x = .005f;
  float y = .005f;
  float z = .005f;
} IncCoords;
