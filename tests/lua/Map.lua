describe("Elona.Map", function()
  describe("should be awesome", function()
    it("should report width and height", function()
          Elona.Testing.setup_map(12, 34)
          assert.are.equal(Elona.Map.width(), 12)
          assert.are.equal(Elona.Map.height(), 34)
    end)
  end)
end)
