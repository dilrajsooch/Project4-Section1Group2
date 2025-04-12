#pragma once
#include "raylib.h"
#include <map>
#include <string>
#include <memory>

class ImageCache {
private:
    static ImageCache* instance;
    std::map<int, Texture2D> textureCache;
    std::map<int, Image> imageCache;
    
    ImageCache() {} // Private constructor for singleton
    
public:
    static ImageCache* GetInstance() {
        if (instance == nullptr) {
            instance = new ImageCache();
        }
        return instance;
    }
    
    // Get a texture for a post ID, creating it if it doesn't exist
    Texture2D GetTexture(int postId, Image image) {
        // Check if texture already exists in cache
        auto it = textureCache.find(postId);
        if (it != textureCache.end()) {
            return it->second;
        }
        
        // Validate the image before processing
        if (image.data == nullptr || image.width <= 0 || image.height <= 0) {
            // Return a default texture or handle the error
            TraceLog(LOG_WARNING, "ImageCache: Invalid image data for post ID %d", postId);
            return Texture2D{0}; // Return an empty texture
        }
        
        // If not in cache, create a new texture
        Image resizedImg = ImageCopy(image);
        if (resizedImg.data == nullptr) {
            TraceLog(LOG_ERROR, "ImageCache: Failed to copy image for post ID %d", postId);
            return Texture2D{0}; // Return an empty texture
        }
        
        ImageResize(&resizedImg, 128, 128);
        Texture2D texture = LoadTextureFromImage(resizedImg);
        
        // Store in cache
        textureCache[postId] = texture;
        imageCache[postId] = resizedImg;
        
        return texture;
    }
    
    // Clean up resources when a post is deleted
    void RemoveTexture(int postId) {
        auto textureIt = textureCache.find(postId);
        auto imageIt = imageCache.find(postId);
        
        if (textureIt != textureCache.end()) {
            UnloadTexture(textureIt->second);
            textureCache.erase(textureIt);
        }
        
        if (imageIt != imageCache.end()) {
            UnloadImage(imageIt->second);
            imageCache.erase(imageIt);
        }
    }
    
    // Clean up all resources
    void Cleanup() {
        for (auto& pair : textureCache) {
            UnloadTexture(pair.second);
        }
        
        for (auto& pair : imageCache) {
            UnloadImage(pair.second);
        }
        
        textureCache.clear();
        imageCache.clear();
    }
    
    ~ImageCache() {
        Cleanup();
    }
};

// Initialize the static instance
ImageCache* ImageCache::instance = nullptr; 